`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/27/2020 12:04:45 PM
// Design Name: 
// Module Name: relu
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


module relu(
    input [31:0] x_data,
    input clk,
    input deriv,
    input rdy,
    output reg [31:0] z_data,
    output reg done
    );
    
    reg zero;
    reg negative;
    reg [2:0] state;
    initial begin
        state <= 4;
        done <= 0;
    end
    always @(posedge clk) begin
        case(state)
            0: //check if our number is negative and check for derivative and check for zero value
                begin
                    if (x_data[30:0] == 0) begin
                        zero <= 1;
                    end else begin
                        zero <= 0;
                    end    
                    if (x_data[31]) begin
                        negative <= 1;
                    end else begin
                        negative <= 0;
                    end
                    if (deriv) begin
                        state <= 2;
                    end else begin
                        state <= 1;
                    end
                end
            1: // if we want relu
                begin
                    if (negative || zero) begin
                        z_data[31] <= x_data[31];
                        z_data[30:0] <= 0;
                        state <= 3;
                    end else begin
                        z_data <= x_data;
                        state <= 3;
                    end
                end 
            2: // if we want relu derivative
                begin
                    if(negative || zero) begin
                        z_data = 32'h00000000; // IEEE-754 representation of 0.
                    end else begin
                         z_data = 32'h3f800000; // IEEE-754 representation of 1.
                    end
                end
            3: // done state
                begin
                    state <= 3;
                    done <= 1;
                end
            4: // wait state
                begin
                    if (rdy) begin
                        state <= 0;
                    end else begin
                        state <= 4;
                    end
                end
         endcase
    end
endmodule
