module bram_rd(
    input                clk        , //ʱ���ź�
    input                rst_n      , //��λ�ź�
    input                start_rd   , //����ʼ�ź�
    input        [31:0]  start_addr , //����ʼ��ַ  
    input        [31:0]  rd_len     , //�����ݵĳ���
    input        [31:0]  rd_freq    , //������Ƶ��
    //RAM�˿�
    output               ram_clk    , //RAMʱ��
    input        [31:0]  ram_rd_data, //RAM�ж���������
    output  reg          ram_en     , //RAMʹ���ź�
    output  reg  [31:0]  ram_addr   , //RAM��ַ
    output       [3:0]   ram_we     , //RAM��д�����ź�
    output  reg  [31:0]  ram_wr_data, //RAMд����
    output               ram_rst      //RAM��λ�ź�,�ߵ�ƽ��Ч
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

//ram_en��ЧʱӦ��һֱ�ڶ���
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
