module MTF_neuron (
    input wire clk,
    input wire reset,
    input wire [7:0] i_ext,
    input wire [7:0] dt,
    input wire [7:0] afn, asp, asn, ausp, dfn, dsp, dsn, dusp,
    output reg [15:0] voltage);

    reg [7:0] current_sum;
    reg [15:0] v_temp;
    reg [15:0] v_x [0:2];
    reg [15:0] i_x [0:3];
    integer i;
    integer j;
    
    reg [7:0] alpha [0:3];
    reg [7:0] delta [0:3];
    reg [15:0] tau [0:3];
    
    always @(posedge clk) begin
        alpha[0] <= afn;
        alpha[1] <= asp;
        alpha[2] <= asn;
        alpha[3] <= ausp;
        
        delta[0] <= dfn;
        delta[1] <= dsp;
        delta[2] <= dsn;
        delta[3] <= dusp;
    end

    always @(posedge clk) begin
        if (reset) begin
            // Initialize internal registers to 0
            v_x[2] = 16'd0;
            v_x[1] = 16'd0;
            v_x[0] = 16'd0;
            i_x[3] = 16'd0;
            i_x[2] = 16'd0;
            i_x[1] = 16'd0;
            i_x[0] = 16'd0;

            voltage <= 16'd0;
            v_temp <= 8'd0;

            current_sum <= 8'd0;
        end
        else begin
            // Calculates each V_x
            v_x[0] <= v_x[0] + ((voltage - v_x[0]));        // Divides by 1 for F time constant
            v_x[1] <= v_x[1] + ((voltage - v_x[1]) >> 7);   // Divides by 128 for S time constant
            v_x[2] <= v_x[2] + ((voltage - v_x[2]) >> 12);  // Divides by 4096 for US time constant
            
            // PWL function to calculate I_x
            for (j = 0; j < 3; j=j+1) begin
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
            end
            
            // Sum of all currents
            current_sum <= i_x[0] + i_x[1] + i_x[2] + i_x[3];
            
            // Calculates news membrane voltage
            voltage <= voltage + (i_ext - voltage - current_sum);
        end
    end
endmodule
