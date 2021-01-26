`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/27/2020 11:48:43 PM
// Design Name: 
// Module Name: adder
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


module adder(
    input [31:0] x_data,
    input [31:0] y_data,
    input clk,
    input x_rdy,
    input y_rdy,
    output reg [31:0] z_data,
    output reg done
    );

    reg x_s, y_s, z_s; // sign
    reg [9:0] x_e, y_e, z_e; // exponent
    reg [26:0] x_m, y_m, z_m; // mantissa
    reg guard, round, sticky;
    reg [27:0] sum;

    reg [3:0] state;
    initial begin
        state <= 0;
        done <= 0;
    end

    always @(posedge clk) begin
        case(state)
            0: // wait for ready
                begin
                    if(x_rdy && y_rdy) begin
                        state <= 1;
                    end
                end
            1: // unpack
                begin
                    x_s <= x_data[31];
                    y_s <= y_data[31];
                    x_e <= x_data[30:23] - 127; //subtract bias
                    y_e <= y_data[30:23] - 127;
                    x_m <= x_data[22:0];
                    y_m <= y_data[22:0];
                    state <= 2;
                end
            2: //special cases
                begin
                    if (x_data[30:0] == 0) begin // if x is zero return y
                        z_data[31] <= y_data[31];
                        z_data[30:23] <= y_data[30:23];
                        z_data[22:0] <= y_data[22:0];
                        state <= 10; // last state
                    end else if (y_data[30:0] == 0) begin // if y is zero return x
                        z_data[31] <= x_data[31];
                        z_data[30:23] <= x_data[30:23];
                        z_data[22:0] <= x_data[22:0];
                        state <= 10; // last state
                    end
                    // Denormalized Number
                    if ($signed(x_e) == -127) begin
                        x_e <= -126;
                    end else begin
                        x_m[26] <= 1;
                    end
                    // Denormalized Number
                    if ($signed(y_e) == -127) begin
                        y_e <= -126;
                    end else begin
                        y_m[26] <= 1;
                    end
                    state <= 3;

                end
            3: // align the exponents of the numbers to be added
                begin
                    if ($signed(x_e) > $signed(y_e)) begin // if x's exponent is greater
                        y_e <= y_e + 1; // add 1 to exponent
                        y_m <= y_m >> 1; // shift mantissa one bit
                        y_m[0] <= y_m[0] | y_m[1];
                    end else if ($signed(y_e) > $signed(x_e)) begin // if y's exponent is greater
                        x_e <= x_e + 1;
                        x_m <= x_m >> 1;
                        x_m[0] <= x_m[0] | x_m[1];
                    end else begin
                        state <= 4; // if the exponents are the same we are ready to move on
                    end
                end
            4: // add
                begin
                    z_e <= x_e;
                    if(x_s == y_s) begin // if the signs are the same
                        sum <= x_m + y_m;
                        z_s <= x_s; 
                    end else begin // if the signs are different
                        if(x_m >= y_m) begin
                            sum <= x_m - y_m;
                            z_s <= x_s;
                        end else begin
                            sum <= y_m - x_m;
                            z_s <= y_s;
                        end
                    end
                    state <= 5;
                end
            5:
                begin
                    if(sum[27]) begin
                        z_m <= sum[27:4];
                        guard <= sum[3];
                        round <= sum[2];
                        sticky <= sum[1] | sum[0];
                        z_e <= z_e + 1;
                    end else begin
                        z_m <= sum[26:3];
                        guard <= sum[2];
                        round <= sum[1];
                        sticky <= sum[0];
                    end
                    state <= 6;
                end
            6: // normalize
                begin
                    if (z_m[23] == 0 && $signed(z_e) > -126) begin
                        z_e <= z_e - 1;
                        z_m <= z_m << 1;
                        z_m[0] <= guard;
                        guard <= round;
                        round <= 0;
                    end else begin
                        state <= 7;
                    end
                end
            7: // normalize
                begin
                    if ($signed(z_e) < -126) begin
                        z_e <= z_e + 1;
                        z_m <= z_m >> 1;
                        guard <= z_m[0];
                        round <= guard;
                        sticky <= sticky | round;
                    end else begin
                        state <= 8;
                    end
                end
            8: // round
                begin
                    if (guard && (round | sticky | z_m[0])) begin
                        z_m <= z_m + 1;
                        if (z_m == 24'hffffff) begin
                            z_e <= z_e + 1;
                        end
                    end
                    state <= 9;
                end
            9: // pack
                begin
                    z_data[22:0] <= z_m[22:0];
                    z_data[30:23] <= z_e[7:0] + 127;
                    z_data[31] <= z_s;
                    if ($signed(z_e) == -126 && z_m[23] == 0) begin
                        z_data[30:23] <= 0;
                    end
                    // possibly fix sign bug
                    state <= 10;
                end
            10: // done
                begin
                    state <= 10;
                    done <= 1;
                end
        endcase
    end
endmodule
