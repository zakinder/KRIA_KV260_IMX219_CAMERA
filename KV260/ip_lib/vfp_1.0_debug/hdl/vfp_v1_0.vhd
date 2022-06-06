library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.constants_package.all;
use work.vpf_records.all;
use work.ports_package.all;
use work.fixed_pkg.all;
use work.float_pkg.all;
entity vfp_v1_0 is
    generic (
        -- System Revision
        revision_number           : std_logic_vector(31 downto 0) := x"05202022";
        C_vfpConfig_DATA_WIDTH    : integer    := 32;
        C_vfpConfig_ADDR_WIDTH    : integer    := 8;
        C_oVideo_TDATA_WIDTH      : integer    := 32;
        C_oVideo_START_COUNT      : integer    := 32;
        C_iVideo_TDATA_WIDTH      : integer    := 32;
        FRAME_PIXEL_DEPTH         : integer    := 10;
        FRAME_WIDTH               : natural    := 1280;
        FRAME_HEIGHT              : natural    := 720);
    port (
        vfpconfig_aclk            : in std_logic;
        vfpconfig_aresetn         : in std_logic;
        vfpconfig_awaddr          : in std_logic_vector(C_vfpConfig_ADDR_WIDTH-1 downto 0);
        vfpconfig_awprot          : in std_logic_vector(2 downto 0);
        vfpconfig_awvalid         : in std_logic;
        vfpconfig_awready         : out std_logic;
        vfpconfig_wdata           : in std_logic_vector(C_vfpConfig_DATA_WIDTH-1 downto 0);
        vfpconfig_wstrb           : in std_logic_vector((C_vfpConfig_DATA_WIDTH/8)-1 downto 0);
        vfpconfig_wvalid          : in std_logic;
        vfpconfig_wready          : out std_logic;
        vfpconfig_bresp           : out std_logic_vector(1 downto 0);
        vfpconfig_bvalid          : out std_logic;
        vfpconfig_bready          : in std_logic;
        vfpconfig_araddr          : in std_logic_vector(C_vfpConfig_ADDR_WIDTH-1 downto 0);
        vfpconfig_arprot          : in std_logic_vector(2 downto 0);
        vfpconfig_arvalid         : in std_logic;
        vfpconfig_arready         : out std_logic;
        vfpconfig_rdata           : out std_logic_vector(C_vfpConfig_DATA_WIDTH-1 downto 0);
        vfpconfig_rresp           : out std_logic_vector(1 downto 0);
        vfpconfig_rvalid          : out std_logic;
        vfpconfig_rready          : in std_logic;
        ovideo_aclk               : in std_logic;
        ovideo_aresetn            : in std_logic;
        ovideo_tvalid             : out std_logic;
        ovideo_tkeep              : out std_logic_vector(3 downto 0);
        ovideo_tdata              : out std_logic_vector(C_oVideo_TDATA_WIDTH-1 downto 0);
        ovideo_tstrb              : out std_logic_vector((C_oVideo_TDATA_WIDTH/8)-1 downto 0);
        ovideo_tlast              : out std_logic;
        ovideo_tready             : in std_logic;
        ovideo_tuser              : out std_logic;

        
        rgb_fr_plw_red            : out std_logic_vector(9 downto 0);
        rgb_fr_plw_gre            : out std_logic_vector(9 downto 0);
        rgb_fr_plw_blu            : out std_logic_vector(9 downto 0);
        rgb_fr_plw_sof            : out std_logic;
        rgb_fr_plw_eol            : out std_logic;
        rgb_fr_plw_eof            : out std_logic;
        rgb_fr_plw_val            : out std_logic;
        rgb_fr_plw_xcnt           : out std_logic_vector(15 downto 0);
        rgb_fr_plw_ycnt           : out std_logic_vector(15 downto 0);
        crd_x                     : out std_logic_vector(15 downto 0);
        crd_y                     : out std_logic_vector(15 downto 0);
        
        ivideo_aclk               : in std_logic;
        ivideo_aresetn            : in std_logic;
        ivideo_tready             : out std_logic;
        ivideo_tkeep              : in std_logic_vector(3 downto 0);  
        ivideo_tdata              : in std_logic_vector(C_iVideo_TDATA_WIDTH-1 downto 0);
        ivideo_tstrb              : in std_logic_vector((C_iVideo_TDATA_WIDTH/8)-1 downto 0);
        ivideo_tlast              : in std_logic;
        ivideo_tuser              : in std_logic; 
        ivideo_tvalid             : in std_logic);
end vfp_v1_0;
architecture arch_imp of vfp_v1_0 is


    signal rgb_to_ccm         : channel;
    signal rgb_fr_ccm         : channel;
    signal rgb_fr_plw         : channel;
    signal rgb_fr_p2w         : channel;
    
    signal rgb_contr1         : channel;
    signal rgb_range1         : channel;
    signal wr_regs            : mRegs;
    signal rd_regs            : mRegs;
    signal cord_x             : std_logic_vector(15 downto 0);
    signal cord_y             : std_logic_vector(15 downto 0);
    signal coefficients       : coefficient_values;
    signal coefficients_out   : coefficient_values;
    signal txCord             : coord;
    signal n_pixel_threshold  : integer := 7;
    signal k_config_number    : integer := 7;
    signal config_number_44   : integer := 0;
    signal config_number_48   : integer := 7;
    signal ccc1                : channel;
    signal ccc2                : channel;
    signal ccc3                : channel;
    signal ccc4                : channel;
    signal ccc5                : channel;
    signal ccc6                : channel;
    signal ccc7                : channel;
    signal ccc8                : channel;
    signal ccm1                : channel;
    signal ccm2                : channel;
    signal ccm3                : channel;
    signal ccm4                : channel;
    signal ccm5                : channel;
    signal ccm6                : channel;
    signal ccm7                : channel;
    signal ccm8                : channel;
    
begin




-- this module encode and decode cpu config data to slave components
vfpConfigInst: vfp_config
generic map(
    revision_number      => revision_number,
    conf_data_width      => C_vfpConfig_DATA_WIDTH,
    conf_addr_width      => C_vfpConfig_ADDR_WIDTH)
port map(
    -- config and requested registers
    wrRegsOut            => wr_regs,
    rdRegsIn             => rd_regs,
    -- cpu config axi4 lite channel
    S_AXI_ACLK           => vfpconfig_aclk,
    S_AXI_ARESETN        => vfpconfig_aresetn,
    S_AXI_AWADDR         => vfpconfig_awaddr,
    S_AXI_AWPROT         => vfpconfig_awprot,
    S_AXI_AWVALID        => vfpconfig_awvalid,
    S_AXI_AWREADY        => vfpconfig_awready,
    S_AXI_WDATA          => vfpconfig_wdata,
    S_AXI_WSTRB          => vfpconfig_wstrb,
    S_AXI_WVALID         => vfpconfig_wvalid,
    S_AXI_WREADY         => vfpconfig_wready,
    S_AXI_BRESP          => vfpconfig_bresp,
    S_AXI_BVALID         => vfpconfig_bvalid,
    S_AXI_BREADY         => vfpconfig_bready,
    S_AXI_ARADDR         => vfpconfig_araddr,
    S_AXI_ARPROT         => vfpconfig_arprot,
    S_AXI_ARVALID        => vfpconfig_arvalid,
    S_AXI_ARREADY        => vfpconfig_arready,
    S_AXI_RDATA          => vfpconfig_rdata,
    S_AXI_RRESP          => vfpconfig_rresp,
    S_AXI_RVALID         => vfpconfig_rvalid,
    S_AXI_RREADY         => vfpconfig_rready);
    
    
VFP_AXI_STREAM_INST : vfp_axi_stream 
generic map(
    TDATA_WIDTH                     => C_iVideo_TDATA_WIDTH,
    FRAME_PIXEL_DEPTH               => FRAME_PIXEL_DEPTH,
    FRAME_WIDTH                     => FRAME_WIDTH,
    FRAME_HEIGHT                    => FRAME_HEIGHT)
port map (
    s_axis_aclk                     => ivideo_aclk,
    s_axis_aresetn                  => ivideo_aresetn,
    s_axis_tready                   => ivideo_tready,
    s_axis_tdata                    => ivideo_tdata,
    s_axis_tlast                    => ivideo_tlast,
    s_axis_tuser                    => ivideo_tuser,
    s_axis_tvalid                   => ivideo_tvalid,
    oCord_x                         => cord_x,
    oCord_y                         => cord_y,
    oRgb                            => rgb_to_ccm);
    

    rgb_fr_plw_xcnt <= std_logic_vector(to_unsigned(rgb_to_ccm.xcnt, 16));
    rgb_fr_plw_ycnt <= std_logic_vector(to_unsigned(rgb_to_ccm.ycnt, 16));
    
    

    txCord.x <= cord_x;
    txCord.y <= cord_y;
    
    crd_x    <= cord_x;
    crd_y    <= cord_y;
    
    coefficients.k1 <= wr_regs.cfigReg1;
    coefficients.k2 <= wr_regs.cfigReg2;
    coefficients.k3 <= wr_regs.cfigReg3;
    coefficients.k4 <= wr_regs.cfigReg4;
    coefficients.k5 <= wr_regs.cfigReg5;
    coefficients.k6 <= wr_regs.cfigReg6;
    coefficients.k7 <= wr_regs.cfigReg7;
    coefficients.k8 <= wr_regs.cfigReg8;
    coefficients.k9 <= wr_regs.cfigReg9;

    
    rd_regs.cfigReg1  <= coefficients_out.k1;
    rd_regs.cfigReg2  <= coefficients_out.k2;
    rd_regs.cfigReg3  <= coefficients_out.k3;
    rd_regs.cfigReg4  <= coefficients_out.k4;
    rd_regs.cfigReg5  <= coefficients_out.k5;
    rd_regs.cfigReg6  <= coefficients_out.k6;
    rd_regs.cfigReg7  <= coefficients_out.k7;
    rd_regs.cfigReg8  <= coefficients_out.k8;
    rd_regs.cfigReg9  <= coefficients_out.k9;

    k_config_number   <= to_integer((unsigned(wr_regs.cfigReg0)));
    n_pixel_threshold <= to_integer((unsigned(wr_regs.cfigReg10)));
    config_number_44  <= to_integer((unsigned(wr_regs.cfigReg11)));
    config_number_48  <= to_integer((unsigned(wr_regs.cfigReg12)));




process (ivideo_aclk)begin
    if rising_edge(ivideo_aclk) then
    if(config_number_44 = 0) then
        ccm1.red           <= rgb_to_ccm.green;
        ccm1.green         <= rgb_to_ccm.red;
        ccm1.blue          <= rgb_to_ccm.blue;
        ccm1.sof           <= rgb_to_ccm.sof;
        ccm1.eol           <= rgb_to_ccm.eol;
        ccm1.eof           <= rgb_to_ccm.eof;
        ccm1.valid         <= rgb_to_ccm.valid;
    elsif(config_number_44 = 1)then
        ccm1.red           <= rgb_to_ccm.red;
        ccm1.green         <= rgb_to_ccm.blue;
        ccm1.blue          <= rgb_to_ccm.green;
        ccm1.sof           <= rgb_to_ccm.sof;
        ccm1.eol           <= rgb_to_ccm.eol;
        ccm1.eof           <= rgb_to_ccm.eof;
        ccm1.valid         <= rgb_to_ccm.valid;
    elsif(config_number_44 = 2)then
        ccm1.red           <= rgb_to_ccm.blue;
        ccm1.green         <= rgb_to_ccm.green;
        ccm1.blue          <= rgb_to_ccm.red;
        ccm1.sof           <= rgb_to_ccm.sof;
        ccm1.eol           <= rgb_to_ccm.eol;
        ccm1.eof           <= rgb_to_ccm.eof;
        ccm1.valid         <= rgb_to_ccm.valid;
    elsif(config_number_44 = 3)then
        ccm1.red           <= rgb_to_ccm.blue;
        ccm1.green         <= rgb_to_ccm.red;
        ccm1.blue          <= rgb_to_ccm.green;
        ccm1.sof           <= rgb_to_ccm.sof;
        ccm1.eol           <= rgb_to_ccm.eol;
        ccm1.eof           <= rgb_to_ccm.eof;
        ccm1.valid         <= rgb_to_ccm.valid;
    else
        ccm1.red           <= rgb_to_ccm.red;
        ccm1.green         <= rgb_to_ccm.green;
        ccm1.blue          <= rgb_to_ccm.blue;
        ccm1.sof           <= rgb_to_ccm.sof;
        ccm1.eol           <= rgb_to_ccm.eol;
        ccm1.eof           <= rgb_to_ccm.eof;
        ccm1.valid         <= rgb_to_ccm.valid;
    end if;
    end if;
end process;










--rgb_range1 <= ccm6;

--recolor_space_2_inst: pixel_localization_9x9_window
--generic map(
--    
--    img_width                   => 1280,
--    i_data_width                => 8)
--port map(
--    clk                         => ivideo_aclk,
--    reset                       => ivideo_aresetn,
--    txCord                      => txCord,
--    neighboring_pixel_threshold => n_pixel_threshold,
--    iRgb                        => rgb_to_ccm,
--    oRgb                        => rgb_fr_plw);
--    
--    
--recolor_space_0_inst: recolor_space_cluster
--generic map(
--    neighboring_pixel_threshold => 10,
--    img_width                   => 1280,
--    i_data_width                => 8)
--port map(
--    clk                => ivideo_aclk,
--    reset              => ivideo_aresetn,
--    iRgb               => rgb_fr_plw,
--    txCord             => txCord,
--    oRgb               => rgb_fr_p2w);
--    
--    
--    
--rgb_contrast_brightness_1_inst: rgb_contrast_brightness_level_1
--generic map (
--    contrast_val  => to_sfixed(0.98,16,-3),
--    exposer_val   => 0)
--port map (                  
--    clk               => ivideo_aclk,
--    rst_l             => ivideo_aresetn,
--    iRgb              => rgb_fr_p2w,
--    oRgb              => rgb_contr1);
--
--rgb_range_1_inst: rgb_range
--generic map (
--    i_data_width       => i_data_width)
--port map (                  
--    clk                => ivideo_aclk,
--    reset              => ivideo_aresetn,
--    iRgb               => rgb_contr1,
--    oRgb               => rgb_range1);
--    
--    


----------------------------------------------------------------------------
---- DARK_CCM
----------------------------------------------------------------------------
--dark_ccm_inst  : ccm_frame
--generic map(
--    k_config_number   => 101)
--port map(
--    clk                 => ivideo_aclk,
--    rst_l               => ivideo_aresetn,
--    iRgb                => ccm1,
--    oRgb                => ccc1);
----------------------------------------------------------------------------
---- LIGHT_CCM
----------------------------------------------------------------------------
--light_ccm_inst  : ccm_frame
--generic map(
--    k_config_number   => 102)
--port map(
--    clk                 => ivideo_aclk,
--    rst_l               => ivideo_aresetn,
--    iRgb                => ccc1,
--    oRgb                => ccc2);
--

balance_ccm_inst  : ccm
port map(
    clk                   => ivideo_aclk,
    rst_l                 => ivideo_aresetn,
    k_config_number       => k_config_number,
    coefficients          => coefficients,
    coefficients_out      => coefficients_out,
    iRgb                  => ccm1,
    oRgb                  => ccm2);
    
    
    
process (ivideo_aclk)begin
    if rising_edge(ivideo_aclk) then
    if(config_number_48 = 0) then
        rgb_fr_ccm           <= ccm1;
    else
        rgb_fr_ccm           <= ccm2;
    end if;
    end if;
end process;

    
   ovideo_tstrb           <= ivideo_tstrb;
   ovideo_tkeep           <= ivideo_tkeep;
   ovideo_tdata           <= "00" & rgb_fr_ccm.red & rgb_fr_ccm.green & rgb_fr_ccm.blue;
   ovideo_tvalid          <= rgb_fr_ccm.valid;
   ovideo_tuser           <= rgb_fr_ccm.sof;
   ovideo_tlast           <= rgb_fr_ccm.eol;
   
   
    rgb_fr_plw_red  <= rgb_fr_ccm.red;
    rgb_fr_plw_gre  <= rgb_fr_ccm.green;
    rgb_fr_plw_blu  <= rgb_fr_ccm.blue;
    rgb_fr_plw_sof  <= rgb_fr_ccm.sof;
    rgb_fr_plw_eol  <= rgb_fr_ccm.eol;
    rgb_fr_plw_eof  <= rgb_fr_ccm.eof;
    rgb_fr_plw_val  <= rgb_fr_ccm.valid;
   
end arch_imp;


