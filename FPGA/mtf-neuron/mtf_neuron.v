module MTF_neuron (
    input wire clk,
    input wire reset,
    input wire [7:0] i_ext,
    input wire [7:0] thresh,
    input wire [7:0] dt,
    input wire [7:0] alpha [3:0],
    input wire [7:0] delta [3:0],
    input wire [15:0] tau [2:0],
    output reg spike,
    output reg [7:0] voltage);

    reg [7:0] current_sum;
    reg [15:0] v_temp;
    reg [15:0] v_x [2:0];
    reg [15:0] i_x [3:0];

    always @(posedge clk) begin
        if (reset) begin
            v_x[2] = 16'd0;
            v_x[1] = 16'd0;
            v_x[0] = 16'd0;
            i_x[3] = 16'd0;
            i_x[2] = 16'd0;
            i_x[1] = 16'd0;
            i_x[0] = 16'd0;

            voltage <= 8'd0;
            v_temp <= 8'd0;

            current_sum <= 8'd0;
            spike <= 0;
        end
        else begin
            voltage <= voltage + ((i_ext*20 - voltage - current_sum) >> 3);
        end
    end

    // Calculating v_x values
    always @(posedge clk) begin
        for (int i = 0; i < 3; i++) begin
            v_x[i] <= v_x[i] + ((dt/tau[i])*(voltage - v_x[i]));
        end
    end

    // Calculating i_x values
    always @(posedge clk) begin
        current_sum <= 8'd0;
        
        for (int j = 0; j < 3; j++) begin
            if (j <= 1) begin v_temp <= v_x[j]; end
            else begin v_temp <= v_x[j-1]; end

            if (v_temp < (-1 + delta[j])) begin
                i_x[j] <= -1*alpha[j];
            end
            else if (v_temp > 1 + delta[j]) begin
                i_x[j] <= alpha[j];
            end
            else begin
                i_x[j] <= alpha[j]*(v_temp - delta[j]);
            end

            current_sum <= current_sum + i_x[j];
        end
    end
endmodule