`timescale 1ns / 1ps

module lif_tb ();
     reg clk;
     reg reset;
     reg [7:0] i_ext = 7'd10;
     reg [7:0] thresh = 7'd20;
     //reg [7:0] tau = 7'd0.001;
     wire spike;
     wire [7:0] voltage;
    
    integer fspike;
    
    LIF_neuron lif1 (
        .clk(clk),
        .reset(reset),
        .i_ext(i_ext),
        .thresh(thresh),
        .tau(tau),
        .spike(spike),
        .voltage(voltage)
    );
    initial
    begin
        clk = 1'b0;
        forever
        begin
            #1 clk = ~clk;
        end
    end 
    
    initial
    begin
        fspike = $fopen("spike.txt","w");

        reset = 1'b1;
        # 10
        reset = 1'b0;
        i_ext = 7'd10;
        # 300
        i_ext = 7'd4;
        # 300
        i_ext = 7'd1;
        # 300
        $fclose(fspike);  
        $finish;
    end
    
    always @(posedge spike)
    begin
//        $display("clk out%d\n",   state);
        $fwrite(fspike,"%t\n",   $realtime);
    end
endmodule
