`timescale 1ns / 1ps

module MTF_neuron_tb ();
     reg clk;
     reg reset;
     reg [7:0] i_ext = 8'd10;
     reg [7:0] thresh = 8'd20;
     reg [7:0] dt;
     reg [7:0] afn, asp, asn, ausp, dfn, dsp, dsn, dusp;
     reg [15:0] tf, ts, tus;
     wire spike;
     wire [7:0] voltage;
    
    integer fspike;
    
    MTF_neuron mtf1 (
        .clk(clk),
        .reset(reset),
        .i_ext(i_ext),
        .thresh(thresh),
        .dt(dt),
        .afn(afn),
        .asp(asp),
        .asn(asn),
        .ausp(ausp),
        .dfn(dfn),
        .dsp(dsp),
        .dsn(dsn),
        .dusp(dusp),
        .tf(tf),
        .ts(ts),
        .tus(tus),
        .spike(spike),
        .voltage(voltage)
    );

    initial
    begin
        clk = 1'b0;

        dt <= ((8'd1) >> 1);            // 0.5

        afn <= -1*8'd2;            // -2
        asp <= 8'd2;               // 2
        asn <= ((-1*8'd3) >> 1);   // -1.5
        ausp <= ((8'd3) >> 1);      // 1

        dfn <= 8'd0;               // 0
        dsp <= 8'd0;               // 0
        dsn <= ((-1*8'd3) >> 1);   // -1.5
        dusp <= ((-1*8'd3) >> 1);   // -1.5

        tf <= 15'd1;                // 1
        ts <= 15'd50;               // 50
        tus <= 15'd2500;             // 2500

        forever
        begin
            #1 clk <= ~clk;
        end
    end 
    
    initial
    begin
        fspike = $fopen("spike.txt","w");

        reset <= 1'b1;
        # 10
        reset <= 1'b0;

        i_ext <= ((-1*8'd3) >> 1);
        # 300
        $fclose(fspike);  
        $finish;
    end
    
    always @(posedge spike)
    begin
        $fwrite(fspike,"%t\n",   $realtime);
    end
endmodule
