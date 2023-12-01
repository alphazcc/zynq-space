module bram_rd(
    input                clk        , //时钟信号
    input                rst_n      , //复位信号
    input                start_rd   , //读开始信号
    input        [31:0]  start_addr , //读开始地址  
    input        [31:0]  rd_len     , //读数据的长度
    input        [31:0]  rd_freq    , //读数据频率
    //RAM端口
    output               ram_clk    , //RAM时钟
    input        [31:0]  ram_rd_data, //RAM中读出的数据
    output  reg          ram_en     , //RAM使能信号
    output  reg  [31:0]  ram_addr   , //RAM地址
    output       [3:0]   ram_we     , //RAM读写控制信号
    output  reg  [31:0]  ram_wr_data, //RAM写数据
    output               ram_rst      //RAM复位信号,高电平有效
);
//reg define

//wire define
wire         pos_start_rd;

assign  ram_rst = 1'b0;
assign  ram_we  = 4'b0;
assign pos_start_rd = start_rd;
assign ram_clk=clk;
//assign ram_en =  1'b1;

wire  add_cnt0 ;
wire  end_cnt0 ;
wire  add_cnt1 ; 
wire  end_cnt1 ; 

reg [31:0]  cnt0;
reg [31:0]  cnt1;

always @(posedge clk or negedge rst_n) begin
    if(!rst_n) begin
        ram_en <= 1'b0;
            end
    else if(pos_start_rd)
            ram_en <= 1'b1;
     else if(!pos_start_rd)
            ram_en <= 1'b0; 
end

//ram_en有效时应该一直在动作
always @(posedge clk or negedge rst_n)begin
    if(!rst_n) begin
        cnt0 <= 0;
    end
    else if(add_cnt0) begin
        if(end_cnt0)
            cnt0 <= 0;
        else
        cnt0 <= cnt0 + 1;
    end
end

assign add_cnt0 =ram_en ;
assign end_cnt0 = add_cnt0 && cnt0==rd_freq-1;

always @(posedge clk or negedge rst_n)begin 
    if(!rst_n)begin
        cnt1 <= 0;
        ram_addr <= start_addr;
    end
    else if(add_cnt1)begin
        if(end_cnt1)begin
            cnt1 <= 0;
            ram_addr <=start_addr;
        end
        else begin
            cnt1 <= cnt1 + 1;
            ram_addr <= ram_addr + 'd4;
        end
    end
end

assign add_cnt1 = end_cnt0;
assign end_cnt1 = add_cnt1 && cnt1>=(rd_len>>2)-1;

endmodule
