`timescale 1ns / 1ps

module MTF_neuron_tb ();
     reg clk;
     reg reset;
     reg [7:0] i_ext = 8'd1;
     reg [7:0] dt;
     reg [7:0] afn, asp, asn, ausp, dfn, dsp, dsn, dusp;
     wire [15:0] voltage;
     
     integer fvmem;
    
    MTF_neuron mtf1 (
        .clk(clk),
        .reset(reset),
        .i_ext(i_ext),
        .dt(dt),
        .afn(afn),
        .asp(asp),
        .asn(asn),
        .ausp(ausp),
        .dfn(dfn),
        .dsp(dsp),
        .dsn(dsn),
        .dusp(dusp),
        .voltage(voltage)
    );

    initial
    begin
        fvmem = $fopen("output.txt","w");
    
        clk = 1'b0;

        // Timestep of 1 second
        dt <= 8'd1;

        // alpha = [-3, 3, -2, 2] ([fn, sp, sn, usp])
        afn <= -1*8'd3;
        asp <= 8'd3;
        asn <= -1*8'd2;
        ausp <= 8'd2;

        // delta = [0, 0, -2, -2] ([fn, sp, sn, usp])
        dfn <= 8'd0;
        dsp <= 8'd0;
        dsn <= -1*8'd2;
        dusp <= -1*8'd2;

        forever
        begin
            #1 clk <= ~clk;
        end
    end 
    
    initial
    begin
        // Simulates MTf neuron at -2 amps of external current
        reset <= 1'b1;
        # 10
        reset <= 1'b0;

        i_ext <= -1*8'd2;
        # 990  
        $fclose(fvmem);
        $finish;
    end
    
    always @(posedge clk)
    begin
        $fwrite(fvmem,"%d\n",voltage);
    end
endmodule
