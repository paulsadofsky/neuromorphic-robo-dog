module MTF_neuron (
    input wire clk,
    input wire reset,
    input wire [7:0] i_ext,
    input wire [7:0] thresh,
    input wire [7:0] alpha [3:0],
    input wire [7:0] delta [3:0],
    input wire [7:0] tau [2:0],
    output reg spike,
    output reg [7:0] voltage);

    reg current_sum;

    always @(posedge clk) begin
        if (reset) begin
            voltage <= 16'd0;
            current_sum <= 0;
            spike <= 0;
        end
        else begin
            voltage <= voltage + ((i_ext*20 - voltage) >> 3);
            
            if (voltage >= thresh) begin
                spike <= 1;
                voltage <= 0;
            end
            else begin
                spike <= 0;
            end
        end
    end
endmodule