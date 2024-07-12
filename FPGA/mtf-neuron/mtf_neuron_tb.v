`timescale 1ns / 1ps

module lif_tb ();
     reg clk;
     reg reset;
     reg [7:0] i_ext = 8'd10;
     reg [7:0] thresh = 8'd20;
     reg [7:0] dt;
     reg [7:0] alpha [3:0];
     reg [7:0] delta [3:0];
     reg [15:0] tau [2:0];
     wire spike;
     wire [7:0] voltage;
    
    integer fspike;
    
    LIF_neuron lif1 (
        .clk(clk),
        .reset(reset),
        .i_ext(i_ext),
        .thresh(thresh),
        .dt(dt),
        .alpha(alpha),
        .delta(delta),
        .tau(tau),
        .spike(spike),
        .voltage(voltage)
    );

    initial
    begin
        clk = 1'b0;

        dt <= ((8'd1) >> 1);            // 0.5

        alpha[0] <= -1*8'd2;            // -2
        alpha[1] <= 8'd2;               // 2
        alpha[2] <= ((-1*8'd3) >> 1);   // -1.5
        alpha[3] <= ((8'd3) >> 1);      // 1

        delta[0] <= 8'd0;               // 0
        delta[1] <= 8'd0;               // 0
        delta[2] <= ((-1*8'd3) >> 1);   // -1.5
        delta[3] <= ((-1*8'd3) >> 1);   // -1.5

        tau[0] <= 15'd1;                // 1
        tau[1] <= 15'd50;               // 50
        tau[2] <= 15'd2500;             // 2500

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
