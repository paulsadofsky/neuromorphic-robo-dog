`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/09/2024 02:52:43 PM
// Design Name: 
// Module Name: lif_neuron
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module lif_neuron(
    input wire clk,
    input wire reset,
    input wire i_ext,
    output reg spike);
    
    parameter tau = 10;
    parameter v_th = 20;
    parameter v_reset = 0;
    
    reg [7:0] v_mem;

    always @(posedge clk)
        begin
        if (reset)
            begin
            v_mem <= 0;
            end
        else
            begin
            v_mem <= v_mem + (i_ext - (v_mem >> tau));
            end
        end
    
    always @(posedge clk)
        begin
        if (v_mem >= v_th)
            begin
            spike <= 1;
            v_mem <= v_reset;
            end
        else
            begin
            spike <= 0;
            end
        end
endmodule
