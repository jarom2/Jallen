`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/25/2020 08:22:35 AM
// Design Name: 
// Module Name: multiplier
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


// this module assumes 32-bit floats with:
// 31: sign bit
// 30-23: exponent (integer part)
// 22-0: fractional part
module multiplier(
    input [31:0] x_data,
    input [31:0] y_data,
    input clk,
    input x_rdy,
    input y_rdy,
    output reg [31:0] z_data,
    output reg done
    );

    reg x_s, y_s, z_s; // sign part
    reg [9:0] x_e, y_e, z_e; // exponent
    reg [23:0] x_m, y_m, z_m; // mantissa
    reg [49:0] product;
    reg guard, sticky, round;

    reg [3:0] state;

    initial begin
        state <= 11;
        done <= 0;
    end
    always @(posedge clk) begin
        case(state)
            0: //unpack
                begin
                    x_e <= x_data[30:23] - 127;
                    y_e <= y_data[30:23] - 127;
                    x_s <= x_data[31];
                    y_s <= y_data[31];
                    x_m <= x_data[22:0];
                    y_m <= y_data[22:0];
                    state <= 1;
                end
            1: // special cases
                begin 
                   if ((x_data[30:0] == 0) || (y_data[30:0] == 0)) begin // if X or Y is zero
                        z_data[31] <= x_s ^ y_s;
                        z_data[30:23] = 0;
                        z_data[22:0] = 0;
                        state <= 10; //goto finish state
                   end else begin
                        state <= 2;
                   end
                   //Denormalized Number
                   if ($signed(x_e) == -127) begin
                        x_e <= -126;
                   end else begin
                        x_m[23] <= 1;
                   end
                   //Denormalized Number
                   if ($signed(y_e) == -127) begin
                        y_e <= -126;
                   end else begin
                        y_m[23] <= 1;
                   end
                end
            2: // normalize x
                begin
                    if (x_m[23]) begin
                        state <= 3;
                    end else begin
                        x_m <= x_m << 1;
                        x_e <= x_e - 1;
                    end  
                end
            3: // normalize y
                begin
                    if (y_m[23]) begin
                        state <= 4;
                    end else begin
                        y_m <= y_m << 1;
                        y_e <= y_e - 1;
                    end
                end
            4: // multiply 
                begin
                    z_s <=  x_s ^ y_s; // XOR the sign bits of the two inputs
                    z_e <= x_e + y_e + 1; // Add exponents
                    product <= x_m * y_m * 4; // Multiply the mantissa parts together
                    state <= 5;
                end
            5:
                begin
                  z_m <= product[49:26];
                  guard <= product[25];
                  round <= product[24];
                  sticky <= (product[23:0]!= 0);
                  state <= 6;
                end
            6:
                begin
                    if (z_m[23] == 0) begin
                        z_e <= z_e - 1;
                        z_m <= z_m << 1;
                        z_m[0] <= guard;
                        guard <= round;
                        round <= 0;
                     end else begin
                        state <= 7;
                     end
                end
            7:
                begin
                    if($signed(z_e) < -126) begin
                       z_e <= z_e + 1;
                       z_m <= z_m >> 1;
                       guard <= z_m[0];
                       round <= guard;
                       sticky <= sticky | round;
                    end else begin
                        state <= 8;
                    end
                end
            8: 
                begin
                    if (guard && (round | sticky | z_m[0])) begin
                        z_m <= z_m + 1;
                        if (z_m == 24'hffffff) begin
                            z_e <= z_e + 1;
                        end
                    end
                    state <= 9;
                end
            9: 
                begin
                    z_data[31] = z_s;
                    z_data[30:23] = z_e + 127;
                    z_data[22:0] = z_m[22:0];
                    state <= 10;
                end
           10: //done state
                begin
                    done <= 1;
                    state <= 10;
                end
           11: //wait state
                begin
                    if (x_rdy && y_rdy) begin
                        state <= 0;
                    end else begin
                        state <= 11;
                    end
                end
        endcase
    end
endmodule
