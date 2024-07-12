`timescale 1ns / 1ps

module lif_tb ();
     reg clk;
     reg reset;
     reg [7:0] i_ext = 8'd10;
     reg [7:0] thresh = 8'd20;
     wire spike;
     wire [7:0] voltage;
    
    integer fspike;
    
    LIF_neuron lif1 (
        .clk(clk),
        .reset(reset),
        .i_ext(i_ext),
        .thresh(thresh),
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
        i_ext = 8'd10;
        # 300
        i_ext = 8'd4;
        # 300
        i_ext = 8'd1;
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
