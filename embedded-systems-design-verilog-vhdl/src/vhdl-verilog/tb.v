`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////
// Company:     University of Verona 
// Engineer:    Mirco De Marchi
// 
// Create Date: 11.12.2019 19:12:29
// Design Name: Test Bench 
// Module Name: tl
// Project Name: First Assignment
// Target Devices: PYNQ
// Tool Versions: 2019.2
// Description: Test bench of VHDL and Verilog floating point multiplier IEEE754
// 
// Dependencies: null
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Test bench module
// Empthy module
module tb();

    parameter SIZE = 32;
    parameter CLOCK_PERIOD = 2;
    
    integer i = 1;
    reg [SIZE-1:0] res1, res2;
    
    // in 
    reg [SIZE-1:0] op1, op2;
    reg [1:0] in_rdy;
    reg clk, rst; 

    // out
    wire [SIZE-1:0] res;
    wire [1:0] res_rdy;

    // file
    integer op_fd;
    // reg[20*8:0] comment;
    
    // Top level connection
    tl dut(
        .op1(op1),
        .op2(op2),
        .in_rdy(in_rdy),
        .res(res),
        .res_rdy(res_rdy),
        .clk(clk),
        .rst(rst)
        );

    // Clock update
    always #(CLOCK_PERIOD/2) clk <= !clk;
    
    ////////////////////////////////////////////////////////////////////////////
    // Test
    initial
    begin
        clk = 1'b0;
        rst = #(CLOCK_PERIOD*2) 1'b1;
        rst = #(CLOCK_PERIOD)   1'b0;
        
        in_rdy = 2'b00;

        // open files
        op_fd = $fopen("test_list.txt", "r");

        if (!op_fd)
        begin
            $display("Error opening files.");
            $finish;
        end

        $display("Floating-point multiplier IEEE754");
        $display("Start simulation\n\n");

        // start reading
        i = 1;
        while (!$feof(op_fd))
        begin
            $fscanf(op_fd, "0x%h 0x%h\n", op1, op2);
            in_rdy = 2'b11;
            #(CLOCK_PERIOD)

            $display("[%d] USER TEST", i);
            $display("\top1=%b op2=%b", op1, op2);

            in_rdy <= 2'b00;

            // Serialize result.
            while (res_rdy == 2'b00) #(CLOCK_PERIOD);
            res1 = res;

            if (res_rdy == 2'b01)
            begin
                $display("\tVerilog result : %b", i, res1);
                while (res_rdy == 2'b01 || res_rdy == 2'b00) #(CLOCK_PERIOD);
                res2 = res;
                $display("\tVHDL result    : %b", i, res2);
            end
            else if (res_rdy == 2'b10)
            begin
                $display("\tVHDL result    : %b", i, res1);
                while (res_rdy == 2'b10 || res_rdy == 2'b00) #(CLOCK_PERIOD);
                res2 <= res;
                $display("\tVerilog result : %b", i, res2);
            end

            if (res1 == res2)
                $display("\tSUCCESS!");
            else
                $display("\tFAIL!");

            i = i + 1;
            #(CLOCK_PERIOD);
            
        end


        // Random test
        $display("");
        while (i < 101)
        begin
            op1 = $urandom;
            op2 = $urandom;
            in_rdy = 2'b11;
            #(CLOCK_PERIOD)

            $display("[%d] RANDOM TEST: op1=%b op2=%b", i, op1, op2);

            in_rdy = 2'b00;

            // Serialize result.
            while (res_rdy == 2'b00) #(CLOCK_PERIOD)

            if (res_rdy == 2'b01)
            begin
                res1 = res;
                $display("\tVerilog result : %b", i, res1);
                while (res_rdy == 2'b01 || res_rdy == 2'b00) #(CLOCK_PERIOD);
                res2 = res;
                $display("\tVHDL result    : %b", i, res2);
            end
            else if (res_rdy == 2'b10)
            begin
                res1 = res;
                $display("\tVHDL result    : %b", i, res1);
                while (res_rdy == 2'b10 || res_rdy == 2'b00) #(CLOCK_PERIOD);
                res2 = res;
                $display("\tVerilog result : %b", i, res2);
            end

            if (res1 == res2)
                $display("\tSUCCESS!");
            else
                $display("\tFAIL!");

            i = i + 1;
            #(CLOCK_PERIOD);
        end

        $fclose(op_fd);
        $display("End simulation\n");

        #(CLOCK_PERIOD*100);
        $finish;
    end
endmodule
