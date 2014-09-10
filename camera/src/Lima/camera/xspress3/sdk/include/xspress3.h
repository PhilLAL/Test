/*
 * xspress3.h
 *
 *  Created on: 19 Mar 2012
 *      Author: wih73
 */
#ifndef XSPRESS3_H_
#define XSPRESS3_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "xspress3_dma_protocol.h"
//#include "Xspress3FemApi.h"
#include "xspress3_data_mod.h"

#define FEM_SINGLE 0
#define FEM_COMPOSITE 1

#define XSP3_BINS_PER_MCA 4096

#define XSP3_SW_NUM_SCALERS 8

#define XSP3_HW_USED_SCALERS 7
#define XSP3_HW_DEFINED_SCALERS 8

#define XSP3_ENERGIES 4096
#define XSP3_MAX_CARDS 8
#define XSP3_MAX_PATH 20
#define XSP3_MAX_IP_CHARS 16
#define XSP3_MAX_CHANS_PER_CARD 9
#define XSP3_MAX_MSG_LEN 1024
#define XSP3_MAX_CHANS (XSP3_MAX_CARDS*XSP3_MAX_CHANS_PER_CARD)
#define XSP3_MAX_ROI 8
#define XSP3_ADC_RANGE 65536
#define XSP3_MAX_TOP_SAMPLES 1024

#define XSP3_TRAILER_LWORDS 2
#define XSP3_10GTX_SOF 0x80000000
#define XSP3_10GTX_EOF 0x40000000
#define XSP3_10GTX_PAD 0x20000000
#define XSP3_10GTX_PACKET_MASK 0x0FFFFFFF
#define XSP3_10GTX_TIMEOUT 30

//! [return codes from library functions]
#define XSP3_OK 				0
#define XSP3_ERROR				-1
#define XSP3_INVALID_PATH		-2
#define XSP3_ILLEGAL_CARD		-3
#define XSP3_ILLEGAL_SUBPATH	-4
#define XSP3_INVALID_DMA_STREAM	-5
#define XSP3_RANGE_CHECK		-6
#define XSP3_INVALID_SCOPE_MOD	-7
#define XSP3_OUT_OF_MEMORY		-8
//! [return codes from library functions]

typedef struct _xspress3_saved_config{
	int ncards;
	int num_tf;
	char* baseIPaddress;
	int basePort;
	char* baseMACaddress;
	int nchan;
	int createModule;
	char* modname;
	int debug;
	int card_index;
	int path;
} Xspress3SavedConfig;

typedef struct _UDPconnection
{
    int socket;                         // File descriptor for socket
    int hostPort;                       // PC Port number
    int femPort;						// FEM port number
    int tos;                            // IP TOS (Type of service) setting
    int recvBuf;                        // Receiving buffer size
    int packetSize;                     // Packet (datagram) size
    int dataSize;                       // Data size for each sending/receiving
    int timeout;						// Time to wait
    int sentPackets;                    // Total UDP packets sent
    int recvPackets;                    // Total UDP packets received
    int sentLoss;                       // Loss packets (send)
    int recvLoss;                       // Loss packets (recv)
    u_int64_t sentBytes;                // Total UDP data sent in bytes
    u_int64_t recvBytes;                // Total UPD data received in bytes
    u_int64_t elapsedTime;              // Elapse time in microsecond
} UDPconnection;

typedef struct _XSPROI
{
		int lhs, rhs, out_bins;
} XSP3Roi;

typedef struct _ChannelDTC
{
	int flags;
	double processDeadTimeAllEventGradient;
	double processDeadTimeAllEventOffset;
	double processDeadTimeInWindowOffset;
	double processDeadTimeInWindowGradient;
} ChannelDTC;

typedef struct _Histogram {
	int path;
	int card;
	int channel;
	MOD_IMAGE *module;
	u_int32_t *buffer;
	u_int32_t bufsiz;
	UDPconnection udpsock;
	int nbins_eng, nbins_aux1, nbins_aux2;
	u_int32_t expected_frame, expected_packet;
	int dropped_frames;
	char busy, reset_frame;
} Histogram;

typedef struct _XSP3Path {
	void* femHandle;
	int type;						// single or multi card system
	int valid;
	int debug;						// print out debug information
	int sub_path[XSP3_MAX_CARDS+1];
	int num_cards;					// number of cards in the system
	int chans_per_card;				// available channels on the card
	int num_chan;					// number of channels requested on the card
	int max_num_chans;				// maximum total number of channels available
	int revision;					// card revision
	int total_lwords_per_card;
	UDPconnection udpsock;
	struct xsp3_scope_data_module *scope_mod;
	pthread_t thread[XSP3_MAX_CHANS_PER_CARD];
	Histogram histogram[XSP3_MAX_CHANS_PER_CARD];
	ChannelDTC dtc[XSP3_MAX_CHANS_PER_CARD];
	double deadtimeEnergy; // in keV NOT eV!
	int run_flags;
//	int num_tf;					// Independent values for scaler and then calcuate for each histogram structure from format and bufsiz
	u_int32_t scaler_start;  	// Start address offset of Scaler data with DRAM on FEM, per card.
	u_int32_t scaler_size; 		// Size in bytes of Scaler data with DRAM on FEM, per card.
	u_int32_t scaler_num_tf;	// Number of time frames of scaler data that will fit with channels set to num_chan.
	int mdio_flags;				// Various options for MDIO access.
	int startingCard;			// Card index of first card, remembered to enable us to make default module names correctly
} XSP3Path;

typedef struct trigger_b_setttings
{
	int avemode;
	int two_over_mode, enable;
	int disable_split, combined;
	int scaled_thres_mode;
	int arm_thres, end_thres, rearm_thres;
	int sep1, sep2, data_delay, event_time;
	int over_thres_delay, over_thres_stretch, over_thres_trim;
} Xspress3_TriggerB;

typedef struct trigger_c_setttings
{
	int arm_thres, end_thres;
	int sep1;
	int avemode;
	int two_over_mode, enb_negative, ignore_near_glitch;
	int over_thres_servo_delay, over_thres_servo_stretch, over_thres_servo_trim;
	int use_det_reset_b, use_det_reset_c;
} Xspress3_TriggerC;

typedef enum {Xsp3TP_Inc, Xsp3TP_IncPlusPeaks} Xsp3TestPattern;

#ifdef __cplusplus
extern "C" {
#endif

/* XSPRESS3 forward declaration*/
int 	xsp3_config(int ncards, int num_tf, char* baseIPaddress, int basePort, char* baseMACaddress, int nchan, int createmodule, char* modname, int debug, int card_index);
int 	xsp3_do_config(int ncards, int num_tf, char* baseIPaddress, int basePort, char* baseMACaddress, int num_chan, int create_module, char* modname, int debug, int card_index);
int		xsp3_config_tcp(char femHostName[][XSP3_MAX_IP_CHARS], int femPort, int card, int chan, int debug);
int 	xsp3_close(int path);
char* 	xsp3_get_error_message();
int		xsp3_get_revision(int path);
int 	xsp3_get_num_chan(int path);
int		xsp3_set_num_chan(int path, int num);
int 	xsp3_get_num_cards(int path);
int 	xsp3_get_chans_per_card(int path);
int 	xsp3_resolve_path(int path, int chan, int *thisPath, int *chanIdx);
int 	xsp3_set_trigger_b(int path, int detector, Xspress3_TriggerB *trig_b);
int 	xsp3_get_trigger_b(int path, int chan, Xspress3_TriggerB *trig_b);
int 	xsp3_set_trigger_c(int path, int detector, Xspress3_TriggerC *trig_c);
int 	xsp3_get_trigger_c(int path, int chan, Xspress3_TriggerC *trig_c);
int 	xsp3_set_trigger_b_ringing(int path, int chan, double scale_a, int delay_a, double scale_b, int delay_b);
int 	xsp3_get_trigger_b_ringing(int path, int chan, double *scale_a, int *delay_a, double *scale_b, int *delay_b);
int 	xsp3_trigger_b_get_ringing_params(int path, int *min_delay, int *max_delay);
int		xsp3_get_max_num_chan(int path);
int 	xsp3_set_window(int path, int chan, int win, int low, int high);
int 	xsp3_set_good_thres(int path, int chan, u_int32_t good_thres);
int 	xsp3_set_trigger_regs_b(int path, int chan, u_int32_t trigb_thres, u_int32_t trigb_timea, u_int32_t trigb_timeb);
int 	xsp3_set_trigger_regs_c(int path, int chan, u_int32_t trigc_otd_servo, u_int32_t trigc_thres);
int 	xsp3_set_chan_cont(int path, int chan, u_int32_t chan_cont);
int 	xsp3_set_format_reg(int path, int chan, u_int32_t format);
int 	xsp3_set_reset(int path, int chan, u_int32_t resetA, u_int32_t resetB, u_int32_t resetC);
int 	xsp3_set_glitch(int path, int chan, u_int32_t glitchA, u_int32_t glitchB);
int 	xsp3_set_servo(int path, int chan, u_int32_t servoA, u_int32_t servoB, u_int32_t servoC);
int 	xsp3_get_window(int path, int chan, int win, u_int32_t *low, u_int32_t *high);
int 	xsp3_get_good_thres(int path, int chan, u_int32_t *good_thres);
int 	xsp3_get_trigger_regs_b(int path, int chan, u_int32_t *trigb_thres, u_int32_t *trigb_timea, u_int32_t *trigb_timeb);
int 	xsp3_get_trigger_regs_c(int path, int chan, u_int32_t *trigc_otd_servo, u_int32_t *trigc_thres);
int 	xsp3_get_glitch(int path, int chan, u_int32_t *glitchA, u_int32_t *glitchB);
int 	xsp3_write_reg(int path, int chan, int region, int offset, int size, u_int32_t *value);
int 	xsp3_read_reg(int path, int chan, int region, int offset, int size, u_int32_t *value);
int 	xsp3_get_servo(int path, int chan, u_int32_t *servoA, u_int32_t *servoB, u_int32_t *servoC);
int 	xsp3_get_chan_cont(int path, int chan, u_int32_t *chan_cont);
int 	xsp3_get_format_reg(int path, int chan, u_int32_t *format);
int 	xsp3_get_format(int path, int chan, int *nbins_eng, int *nbins_aux1, int *nbins_aux2, int * nbins_tf);
int 	xsp3_get_reset(int path, int chan, u_int32_t *resetA, u_int32_t *resetB, u_int32_t * resetC);
int 	xsp3_write_glob_reg(int path, int card, int offset, int size, u_int32_t *value);
int 	xsp3_read_glob_reg(int path, int card, int offset, int size, u_int32_t *value);
int 	xsp3_read_raw_reg(int path, int card, u_int32_t address, int size, u_int32_t *value);
int		xsp3_set_clock_control(int path, int card, u_int32_t clock);
int		xsp3_get_clock_control(int path, int card, u_int32_t *clock);
int		xsp3_set_scope(int path, int card, u_int32_t scope_chn, u_int32_t scope_src, u_int32_t scope_nwd, u_int32_t scope_alt);
int 	xsp3_get_scope(int path, int card, u_int32_t *scope_chn, u_int32_t *scope_src, u_int32_t *scope_nwd, u_int32_t *scope_alt);
int 	xsp3_scope_settings_from_mod(int path);
int 	xsp3_scope_settings_to_mod(int path);
int 	xsp3_set_scope_stream(int path, int card, int stream, u_int32_t chan, u_int32_t src, u_int32_t alt);
int 	xsp3_set_glob_timeA(int path, int card, u_int32_t time);
int 	xsp3_set_glob_timeFixed(int path, int card, u_int32_t time);
int 	xsp3_get_glob_timeA(int path, int card, u_int32_t *time);
int 	xsp3_get_glob_timeFixed(int path, int card, u_int32_t *time);
int		xsp3_dma_reset(int path, int card, u_int32_t function_mask);
int 	xsp3_dma_config_memory(int path, int card, int layout);
int 	xsp3_dma_build_desc(int path, int card, u_int32_t func, XSP3_DMA_MsgBuildDesc *msg);
int 	xsp3_dma_build_debug_desc(int path, int card, u_int32_t stream, XSP3_DMA_MsgBuildDebugDesc *msg);
int 	xsp3_dma_start(int path, int card, u_int32_t stream, XSP3_DMA_MsgStart *msgStart);
int 	xsp3_dma_build_test_pat(int path, int card, u_int32_t func, XSP3_DMA_MsgTestPat *msg);
int 	xsp3_dma_print_data(int path, int card, u_int32_t func, XSP3_DMA_MsgPrint *msg);
int 	xsp3_dma_print_scope_data(int path, int card, XSP3_DMA_MsgPrint *msg);
int 	xsp3_dma_print_desc(int path, int card, u_int32_t stream, XSP3_DMA_MsgPrintDesc *msg);
int 	xsp3_get_dma_status_block(int path, int card, XSP3_DMA_StatusBlock *statusBlock);
int		xsp3_dma_check_desc(int path, int card, u_int32_t stream, XSP3_DMA_MsgCheckDesc *msg);
int 	xsp3_dma_resend(int path, int card, u_int32_t stream, u_int32_t first, u_int32_t num);
int 	xsp3_dma_read_status(int path, int card, u_int32_t stream_mask);
int 	xsp3_scope_wait(int path, int card);
int		xsp3_config_udp(int path, int card, char *femMACaddress, char*femIPaddress, int femPort, char* hostIPaddress, int hostPort);
int 	xsp3_config_histogram_udp(int path, int card, char *hostIPaddress, int hostPort, char *femIPaddress, int femPort);
int		xsp3_set_udp_port(int path, int card, int hostPort);
int 	xsp3_set_udp_packet_size(int path, int card, int size_bytes);
int		xsp3_read_scope_data(int path, int card);
int		xsp3_read_scope_data_int(int path, int card, int swap);
int		xsp3_create_data_module (int path, char* modname);
int		xsp3_read_rdma_reg(int path, int card, int address, int size, u_int32_t *value);
int		xsp3_write_rdma_reg(int path, int card, int address, int size, u_int32_t *value);
int		xsp3_read_spi_reg(int path, int card, int address, int size, u_int32_t *value);
int		xsp3_write_spi_reg(int path, int card, int address, int size, u_int32_t *value);
int		xsp3_write_dram(int path, int card, int address, int size, u_int32_t* value);
int 	xsp3_reset_10g_frame_counter(int path, int card);
int	 	xsp3_read_data_10g(int path, int card, int stream, int offset_bytes, int size_bytes, unsigned char *buff);
int 	xsp3_read_data_10g_receive(int path, int card, XSP3_DMA_StatusBlock * statusBlock, unsigned char *buff_base, u_int32_t  first_frame, u_int32_t num_frames, u_int32_t *frame_flags);
int		xsp3_write_playback_data(int path, int card, u_int32_t* buffer, size_t nbytes);
int 	xsp3_write_data_10g(int path, int card, u_int32_t* buffer, int dst_stream, int offset_bytes, size_t nbytes);
int 	xsp3_histogram_mkmod(int path, int chan, char *root_name, int num_tf);
int		xsp3_histogram_start(int path, int card);
int		xsp3_histogram_arm(int path, int card);
int 	xsp3_histogram_start_count_enb(int path, int card, int count_enb);
int		xsp3_histogram_stop(int path, int card);
int 	xsp3_histogram_clear(int path, int first_chan, int num_chan, int first_frame, int num_frames);
int 	xsp3_histogram_read4d(int path, u_int32_t *buffer, unsigned eng, unsigned aux, unsigned chan, unsigned tf, unsigned num_eng, unsigned num_aux, unsigned num_chan, unsigned num_tf);
int 	xsp3_histogram_read3d(int path, u_int32_t *buffer,	unsigned x, unsigned y, unsigned t, unsigned dx, unsigned dy, unsigned dt);
int 	xsp3_histogram_read_chan(int path, u_int32_t *buffer, unsigned chan, unsigned eng, unsigned aux, unsigned tf, unsigned num_eng, unsigned num_aux, unsigned num_tf);
int 	xsp3_histogram_write_test_pat(int path, Xsp3TestPattern type);
int 	xsp3_histogram_get_dropped(int path, int chan);
int 	xsp3_histogram_is_busy(int path, int chan);
int 	xsp3_scaler_get_num_tf(int path);
int 	xsp3_scaler_check_desc(int path, int card);
int 	xsp3_scaler_read(int path, u_int32_t *dest, unsigned scaler, unsigned chan, unsigned t, unsigned n_scalers, unsigned n_chan, unsigned dt);
int 	xsp3_config_scaler(int path);
int 	xsp3_scaler_dtc_read(int path, double *dest, unsigned scaler, unsigned chan, unsigned t, unsigned n_scalers,
			unsigned n_chan, unsigned dt);
int 	xsp3_hist_dtc_read4d(int path, double *hist_buff, double *scal_buff, unsigned eng, unsigned aux, unsigned chan, unsigned tf,
			unsigned num_eng, unsigned num_aux, unsigned num_chan, unsigned num_tf);
int		xsp3_setDeadtimeCalculationEnergy(int path, double energy);
double 	xsp3_getDeadtimeCalculationEnergy(int path);
int 	xsp3_setDeadtimeCorrectionParameters(int path, int chan, int flags, double processDeadTimeAllEventGradient,
			double processDeadTimeAllEventOffset, double processDeadTimeInWindowOffset, double processDeadTimeInWindowGradient);
int		xsp3_getDeadtimeCorrectionParameters(int path, int chan, int *flags, double *processDeadTimeAllEventGradient,
			double *processDeadTimeAllEventOffset, double *processDeadTimeInWindowOffset, double *processDeadTimeInWindowGradient);
int 	xsp3_getDeadtimeCorrectionFlags(int path, int chan, int *flags);
int 	xsp3_calculateDeadtimeCorrectionFactors(int path, u_int32_t* hardwareScalerReadings, double* dtcFactors, double *inpEst, int num_tf, int num_chan);

int 	xsp3_set_roi(int path, int chan, int num_roi, XSP3Roi *roi);
int 	xsp3_init_roi(int path, int chan);

MOD_IMAGE *xsp3_mkmod(char *name, u_int32_t num_x, u_int32_t num_y, char *x_lab, char *y_lab, int data_float, mh_com **mod_head);
int 	xsp3_set_data_module(int path, int chan, MOD_IMAGE* module, u_int32_t* buffer, u_int32_t bufsiz);
MOD_IMAGE* xsp3_get_data_module(int path, int chan);

struct	xsp3_scope_data_module * xsp3_scope_get_module(int path); 

int 	xsp3_system_start(int path, int card);
int 	xsp3_system_arm(int path, int card);
int 	xsp3_histogram_continue(int path, int card);
int 	xsp3_histogram_pause(int path, int card);

int 	xsp3_system_start_count_enb(int path, int card, int count_enb);
int     xsp3_set_run_flags(int path, int flags);
int     xsp3_get_run_flags(int path);
int		xsp3_get_bins_per_mca(int path);
int 	xsp3_bram_init(int path, int chan, int region, double scaling);
int 	xsp3_register_init(int path, int chan);

int 	xsp3_mdio_display(int path, int card);
int 	xsp3_mdio_set_connection(int path, int card, int conn);
int 	xsp3_mdio_write(int path, int card, int port, int device, int addr, int data);
int 	xsp3_mdio_read(int path, int card, int port, int device, int addr, u_int32_t *data);
int 	xsp3_mdio_read_inc(int path, int card, int port, int device, u_int32_t *data);

int 	xsp3_clocks_setup(int path, int card, int clk_src, int flags, int tp_type);
int		xsp3_set_ppc_debuglevel(int path, int card, int ppc1, int ppc2, int level);

//int 	xsp3_get_resmode(int path, int chan, int *res_mode, int *res_thres);
int 	xsp3_get_aux1_mode(int path, int chan, int *aux1_mode, int *aux1_thres);

int 	xsp3_nbits_aux1(int path, int res_mode);
int 	xsp3_nbits_adc(int path, int adc_cont);
int 	xsp3_get_max_ave(int path, int chan);
int 	xsp3_trigger_b_get_diff_params(int path, int *sep_offset, int *sep_max);
int 	xsp3_format_run(int path, int chan, int aux1_mode, int res_thres, int aux2_cont, int disables, int aux2_mode, int nbits_eng);
int 	xsp3_set_data_buffer(int path, int chan, u_int32_t*buffer, u_int32_t bufsiz);
int 	xsp3_get_num_tf(int path);
int 	xsp3_set_num_tf(int path, int num_tf);

int 	xsp3_set_scope_search(int path, int chan, u_int32_t scope_search);
int 	xsp3_get_scope_search(int path, int chan, u_int32_t *scope_search);

int 	xsp3_save_settings(int path, char *dir_name);
int 	xsp3_restore_settings(int path, char *dir_name, int force_mismatch);
int 	xsp3_get_glob_time_statA(int path, int card, u_int32_t *time);

int 	xsp3_set_data_mux_cont(int path, int card, u_int32_t mux_cont);
int 	xsp3_get_data_mux_cont(int path, int card, u_int32_t *mux_cont);


int 	xsp3_i2c_read_adc_temp(int path, int card, float *temp);
int 	xsp3_i2c_set_adc_temp_limit(int path, int card, int critTemp);
int 	xsp3_write_fan_cont(int path, int card, int offset, int size, u_int32_t* value);
int 	xsp3_read_fan_cont(int path, int card, int offset, int size, u_int32_t *value);

int 	xsp3_playback_load_x2(int path, int card, char *filename, int do_test, int do_scale, int do_swap);
int 	xsp3_playback_load_x3(int path, int card, char *filename, int src0, int src1, int file_streams, int digital);

#ifdef __cplusplus
}
#endif


/* XSPRESS3 code */
#define XSP3_REVISION		0   /* Revision Number D[31..16] = major revision, D[15..0] = minor revision	*/
#define XSP3_REG_PRESENT   	1   /* Read/Writeable register present Mask bit 0 = Register offset 0		*/
#define XSP3_CHAN_CONT		2   /* General Chan Control functions										*/
#define XSP3_TP_CYCLES   	3   /* Test pattern generator cycles register when present					*/
#define XSP3_RESETA			4	/* Reset Control A														*/
#define XSP3_RESETB			5	/* Reset Control B														*/
#define XSP3_RESETC			6	/* Reset Control C														*/
#define XSP3_GLITCHA 		7   /* Glitch control A														*/
#define XSP3_GLITCHB 		8   /* Glitch control B														*/
#define XSP3_TRIGB_THRES	10	/* Individual Trigger B Threshold control								*/
#define XSP3_TRIGB_TIMEA 	11	/* Individual Trigger B TimesA 											*/
#define XSP3_TRIGB_TIMEB	12	/* Individual Trigger B TimesB 											*/
#define XSP3_TRIGBC_TIME 	13	/* Individual Trigger B or C OTD servo  Times							*/
#define XSP3_TRIGC_THRES 	14	/* Individual Trigger C Threshold and averaging							*/
#define XSP3_TRIGB_RINGING	15	/* Ringing removal for Trigger B 1st differential 						*/

#define XSP3_CAL_CONT		16    /* Cal Cont															*/

#define XSP3_SERVO_CONT_A	17	/* Servo Control A 														*/
#define XSP3_SERVO_CONT_B	18	/* Servo Control B 														*/
#define XSP3_SERVO_CONT_C	19	/* Servo Control C 														*/

#define XSP3_WINDOW0_THRES	24   	/* Window values for Window 0 Scaler	 							*/
#define XSP3_WINDOW1_THRES  25   	/* Window values for Window 1 Scaler								*/
#define XSP3_GOOD_THRES 	26   	/* Window values for Good Event Scaler								*/

#define XSP3_FORMAT			28	/* Format Control 														*/	
#define XSP3_SCOPE_SEARCH  	29	/* Special scope search aid, when configured         					*/

#define XSP_MAX_NUM_CHAN_REG	32			// Maximum number of writeable channel registers use for initialise

#define XSP3_LIVE_TIME_SCAL		(32+0) 	   	/* Total Time Scaler direct readback			*/
#define XSP3_RESET_TICKS_SCAL	(32+1) 	 	/* Reset Ticks Scaler direct readback			*/
#define XSP3_RESET_COUNT_SCAL	(32+2) 	 	/* Reset Count Scaler direct readback			*/
#define XSP3_ALL_EVENT_SCAL 	(32+3)		/* All event Scaler direct readback			*/
#define XSP3_GOOD_EVENT_SCAL 	(32+4)		/* All event Scaler direct readback			*/
#define XSP3_IN_WIDNOW0_SCAL 	(32+5)		/* All event Scaler direct readback			*/
#define XSP3_IN_WIDNOW1_SCAL 	(32+6)		/* All event Scaler direct readback			*/

#define XSP3_MAX_NUM_READ_CHAN_REG 39

#define XSP3_REVISION_GET_MAJOR(x) (((x)&0xFFFF)>>16)
#define XSP3_REVISION_GET_MINOR(x) ((x)&0xFFFF)

#define XSP3_CC_SEL_DATA(x)			((x)&7)
#define XSP3_CC_SEL_DATA_NORMAL			0
#define XSP3_CC_SEL_DATA_ALTERNATE		1
#define XSP3_CC_SEL_DATA_MUX_DATA		2
#define XSP3_CC_SEL_DATA_EXT0			4
#define XSP3_CC_SEL_DATA_EXT1			5

//! [XSP3_CC_REGISTER]
#define XSP3_CC_DATA_INV				(1<<3)				//!< 1's complement the data when adc data ramps from high to low
#define XSP3_CC_DET_RESET_INV			(1<<4)
#define XSP3_CC_USE_TEST_PAT			(1<<5)
#define XSP3_CC_TP_CONTINUOUS			(1<<6)
#define XSP3_CC_USE_RESET_FROM_DIFF		(1<<7)
#define XSP3_CC_GR_MASKS_RESET_TICKS	(1<<8)
#define XSP3_CC_GR_FROM_PLAYBACK		(1<<9)
#define XSP3_CC_ENB_INL_CORR			(1<<10)
#define XSP3_CC_AVE_RINGING_REMOVED		(1<<11)
#define XSP3_CC_LIVE_TICKS_MODE(x)		(((x)&7)<<12)
#define XSP3_CC_GOOD_GRADE_MODE(x)		(((x)&3)<<15)
#define XSP3_CC_NEB_EVENT_MODE(x)		(((x)&7)<<20)
#define XSP3_CC_MAX_FILT_LEN(x)			(((x)&7)<<24)
#define XSP3_CC_GET_MAX_FILT_LEN(x)		(((x)>>24)&7)
#define XSP3_CC_SEL_ENERGY(x)			(((x)&0xF)<<28)

#define XSP3_CC_LT_OFF						0		//!< Live ticks are not counter on scalers.
#define XSP3_CC_LT_RESET_TICKS				1		//!< Live ticks are counted on Reset Ticks Counter (cannot compare with standard DTC, but suitable for run modes
#define XSP3_CC_LT_IN_WINDOW1				2 		//!< Live ticks are counted in the in-window1 counter. Both DTC techniques can be tried.
#define XSP3_CC_LT_PILEUP					3 		//!< Live ticks are counted in the pile-up counter.    Both DTC techniques can be tried.

#define XSP3_CC_GOOD_GRADE_MCA_ONLY			0		//!< Good grade masks MCA only. Scalers unchanged
#define XSP3_CC_GOOD_GRADE_MCA_WIN			1		//!< Good grade masks MCA and inWin scalers. Correct with increased dead time
#define XSP3_CC_GOOD_GRADE_ALL_EVENT		2		//!< Good grade masks MCA and Inwin 0, 1. AllEvent and all good become AllEvent and AllGoodGrade.
#define XSP3_CC_GOOD_GRADE_ALL_GOOD 		3		//!< Good grade masks MCA and Inwin 0, 1. AllEvent and all good become AllGood and AllGood_GoodGrade.
//! [XSP3_CC_REGISTER]

//! [XSP3_RESET_A_REGISTER]
#define XSP3_RESET_TIME(x)				(((x)&0x3ff)<<0)
#define XSP3_RESET_OUTPUT_RESET			(1<<10)
#define XSP3_RESET_OVER_RANGE_EXTEND	(1<<11)
#define XSP3_RESET_DELAY(x)				(((x)&0xf)<<12)
#define XSP3_RESET_THRES(x)				(((x)&0xFFFF)<<16)

#define XSP3_RESET_GET_TIME(x)			(((x)>>0)&0x3ff)
#define XSP3_RESET_GET_DELAY(x)			(((x)>>12)&0xf)
#define XSP3_RESET_GET_THRES(x)			(((x)>>16)&0xFFFF)
//! [XSP3_RESET_A_REGISTER]

#define XSP3_RESET_MAX_RESET_TIME 0x3FF
#define XSP3_RESET_MAX_RESET_THRES 0xFFFF

#define XSP3_RESET_MAX_DIFF_DEL 0xF
#define XSP3_RESET_MAX_DATA_DEL 0x3F
#define XSP3_RESET_MIN_DIFF_THRES (-2048*16)
#define XSP3_RESET_MAX_RAW_TIME 0x3F

//! [XSP3_RESET_B_REGISTER]
#define XSP3_RESET_B_DIFF_DEL(x)		(((x)&0xF)<<0)	
#define XSP3_RESET_B_DATA_DEL(x)		(((x)&0x3F)<<4)
#define XSP3_RESET_B_AVE_CONT(x)		(((x)&0x3)<<10)
#define XSP3_RESET_B_DIFF_THRES(x)		((((x)>>4)&0x7FF)<<12)
#define XSP3_RESET_B_RAW_TIME(x)		(((x)&0x3F)<<23)
#define XSP3_RESET_B_SET_MODE(x)		(((x)&0x3)<<30)

//! [XSP3_RESET_B_REGISTER]

#define XSP3_RESET_B_MODE_OFF			0
#define XSP3_RESET_B_MODE_FROM_START	1
#define XSP3_RESET_B_MODE_END_GRAD		2
#define XSP3_RESET_B_MODE_END_LEVEL		3

#define XSP3_RESET_B_SUM1		0
#define XSP3_RESET_B_SUM2		1
#define XSP3_RESET_B_SUM4		2

//! [XSP3_RESET_C_REGISTER]
#define XSP3_RESET_C_THRES_FALL(x)		(((x)&0xFFFF)<<16)
#define XSP3_RESET_C_THRES_RISE(x)		(((x)&0xFFFF)<<0)
#define XSP3_RESET_C_GET_THRES_FALL(x)		(((x)>>16)&0xFFFF)
#define XSP3_RESET_C_GET_THRES_RISE(x)		((x)&0xFFFF)
//! [XSP3_RESET_C_REGISTER]

//! [XSP3_GLITCH_A_REGISTER]
#define XSP3_GLITCH_A_ENABLE					(1<<0)
#define XSP3_GLITCH_A_GLITCH_TIME(x)			(((x)&0x1ff)<<1)
#define XSP3_GLITCH_A_GLITCH_THRES(x)			(((x)&0xff)<<10)
#define XSP3_GLITCH_A_DIFF_TIME(x)				(((x)&0x1)<<18)
#define XSP3_GLITCH_A_PAD_MODE(x)				(((x)&0x1)<<19)  // Appears not to be supported in HW
#define XSP3_GLITCH_A_PRE_TIME(x)				(((x)&0x1f)<<20)
#define XSP3_GLITCH_A_FROM_GLOBAL_RESET_LEVEL	(1<<25)
#define XSP3_GLITCH_A_FROM_GLOBAL_RESET_EDGE	(1<<26)
#define XSP3_GLITCH_A_RETRIGGER					(1<<27)
#define XSP3_GLITCH_A_MASK_GLITCH_EVENT			(1<<28)
#define XSP3_GLITCH_A_COUNT_GLICTH_TIME			(1<<29)
#define XSP3_GLITCH_A_EVDET(x)					(((x)&0x3)<<30)
//! [XSP3_GLITCH_A_REGISTER]

#define XSP3_GLITCH_A_GET_GLITCH_TIME(x)  (((x)>>1)&0x1ff)
#define XSP3_GLITCH_A_GET_GLITCH_THRES(x) (((x)>>10)&0xff)

#define XSP3_GLITCH_A_MAX_GLITCH_TIME 0x1FF
/* Max glitch time was 0x3F in early releases, but discouraging such operation and not supported in newer firmware */
#define XSP3_GLITCH_A_MAX_PRE_TIME    0x1F

//! [XSP3_GLITCH_B_REGISTER]
#define XSP3_GLITCH_B_HOLDOFF(x)			((x)&0x1FF)
#define XSP3_GLITCH_B_SUMMODE(x)			(((x)&3)<<9) // XSP3_GLITCH_B_SUM1 | XSP3_GLITCH_B_SUM2 | XSP3_GLITCH_B_SUM4
#define XSP3_GLITCH_B_GET_SUMMODE(x)		(((x)>>9)&3)
#define XSP3_GLITCH_B_MASK_BOG_EVENT		(1<<11)
#define XSP3_GLITCH_B_GLITCH_STRETCH(x)		(((x)&0x3F)<<12)
#define XSP3_GLITCH_B_GET_GLITCH_STRETCH(x)	(((x)>>12)&0x3F)
#define XSP3_GLITCH_B_BGE_STRETCH(x)		(((x)&0x1F)<<18)
#define XSP3_GLITCH_B_GET_BGE_STRETCH(x)	(((x)>>18)&0x1F))
#define XSP3_GLITCH_B_DIFF_SEP(x)			(((x)&0xF)<<24)
#define XSP3_GLITCH_B_GET_DIFF_SEP(x)		(((x)>>24)&0xF)
#define XSP3_GLITCH_B_FORCE_EVENT			(1<<23)
#define XSP3_GLITCH_B_BGE_EXTEND			(1<<28)
#define XSP3_GLITCH_B_SHORT_PADDING			(1<<29)
#define XSP3_GLITCH_B_BGE_LOOKAHEAD			(1<<30)
//! [XSP3_GLITCH_B_REGISTER]

#define XSP3_GLITCH_B_SUM1		0
#define XSP3_GLITCH_B_SUM2		1
#define XSP3_GLITCH_B_SUM4		2

#define XSP3_GLITCH_B_MAX_HOLDOFF 0x1FF
#define XSP3_GLITCH_B_MAX_STRETCH 0x3f
#define XSP3_GLITCH_B_MAX_BGE_STRETCH 0x1f
#define XSP3_GLITCH_B_MAX_DIFF_SEP    15


//! [XSP3_TRIGGERB_THRESHOLD_REGISTER]
#define X3TRIG_B_THRES_SET_ARM(x)	(((x)&0x3FF))
#define X3TRIG_B_THRES_SET_END(x)	(((x)&0x3FF)<<10)
#define X3TRIG_B_THRES_SET_REARM(x)	(((x)&0x3FF)<<20)
#define X3TRIG_B_THRES_TWO_OVER		(1<<30)
#define X3TRIG_B_THRES_ENABLE_CFD	(1<<31)
#define X3TRIG_B_THRES_GET_ARM(x)	(((x)&0x3FF))
#define X3TRIG_B_THRES_GET_END(x)	(((x)>>10)&0x3FF)
#define X3TRIG_B_THRES_GET_REARM(x)	(((x)>>20)&0x3FF)
//! [XSP3_TRIGGERB_THRESHOLD_REGISTER]

#define X3TRIG_B_ENABLE_CFD     1		/* used as argument to setup-trigger_b function */
#define X3TRIG_B_ENABLE_OVERTHR 2		/* used as argument to setup-trigger_b function */

//! [XSP3_TRIGGERB_TIMEA_REGISTER]
#define X3TRIG_B_TIME_SET_DIFF1(x)		(((x)&0x7f))		// First differential delay. Value 0 creates delay of 3
#define X3TRIG_B_TIME_SET_DIFF2(x)		(((x)&0x7f)<<7) 	// Second differential delay. Value 0 creates delay of 3
#define X3TRIG_B_TIME_SET_DATA(x)		(((x)&0x7f)<<14)	// Data delay. Value 0 creates delay of 3
#define X3TRIG_B_TIME_SET_EVENT(x)		(((x)&0x7f)<<21)	// Event width. Value 1 creates width of 1
#define X3TRIG_B_TIME_SET_AVEMODE(x)	(((x)&3)<<28)		// Average mode
#define X3TRIG_B_TIME_DISABLE_SPLIT		(1<<30)
#define X3TRIG_B_TIME_ENABLE_OVERTHR	(1<<31)

#define X3TRIG_B_TIME_GET_DIFF1(x)		(((x)&0x7f))		/* First differential delay. Value 0 creates delay of 3 */
#define X3TRIG_B_TIME_GET_DIFF2(x)		(((x)>>7)&0x7f) 	/* Second differential delay. Value 0 creates delay of 3 */
#define X3TRIG_B_TIME_GET_DATA(x)		(((x)>>14)&0x7f)	/* Data delay. Value 0 creates delay of 3 */
#define X3TRIG_B_TIME_GET_EVENT(x)		(((x)>>21)&0x7f)	/* Event width. Value 1 creates width of 1 */
#define X3TRIG_B_TIME_GET_AVEMODE(x)	(((x)>>28)&3)		/* Average mode */

#define X3TRIG_B_TIME_DIFF_OFFSET_BRAM 	3					// Actual deta for value 0 when using BRAM delay
#define X3TRIG_B_TIME_DIFF_OFFSET_SRL 	1					// Actual deta for value 0 when using SRL32 delay
#define X3TRIG_B_TIME_DIFF_MAX_BRAM 	0x7F				// Maximum delay value when using BRAM Delay
#define X3TRIG_B_TIME_DIFF_MAX_SRL 		0x1F				// Maximum delay value when using SRL32 Delay
#define X3TRIG_B_TIME_DATA_DELAY_OFFSET	3					// Offset to apply to Data delay (using BRAM delay)
#define X3TRIG_B_MAX_DATA_DEL		0x7F

//! [XSP3_TRIGGERB_TIMEA_REGISTER]

//! [XSP3_TRIGGERB_OVER_THRESHOLD_REGISTER]
#define X3TRIG_B_SET_OVERTHR_DELAY(x)	(((x)&0x1F))
#define X3TRIG_B_SET_OVERTHR_STRETCH(x)	(((x)&0x3F)<<8)
#define X3TRIG_B_SET_OVERTHR_TRIM(x)	(((x)&0x1F)<<16)
#define X3TRIG_B_GET_OVERTHR_DELAY(x)	(((x)&0x1F))
#define X3TRIG_B_GET_OVERTHR_STRETCH(x)	(((x)>>8)&0x3F)
#define X3TRIG_B_GET_OVERTHR_TRIM(x)	(((x)>>16)&0x1F)
//! [XSP3_TRIGGERB_OVER_THRESHOLD_REGISTER]

//! [XSP3_TRIGGERB_TIMEB_REGISTER]
#define X3TRIG_B_TIMEB_SET_THRES_SCALE(x)	(((x)&1)<<30)
#define X3TRIG_B_TIMEB_SCALED_THRES_MODE	(1<<31)
#define X3TRIG_B_TIMEB_GET_THRES_SCALE(x) (((x)>>30)&1)
//! [XSP3_TRIGGERB_TIMEB_REGISTER]

//! [XSP3_TRIGGERBC_OVER_THRESHOLD_SERVO_REGISTER]
#define X3TRIG_BC_SET_OVERTHR_SERVO_DELAY(x)    (((x)&0x1F))
#define X3TRIG_BC_SET_OVERTHR_SERVO_STRETCH(x)  (((x)&0x3F)<<8)
#define X3TRIG_BC_GET_OVERTHR_SERVO_DELAY(x)    (((x)&0x1F))
#define X3TRIG_BC_GET_OVERTHR_SERVO_STRETCH(x)  (((x)>>8)&0x3F)

#define X3TRIG_BC_OVERTHR_SERVO_USE_DET_RESET  	(1<<31)			// Use DeResetTrimmed to disable Over threshold for servo trigger during detector reset (usually Unnecessary)
#define X3TRIG_BC_OVERTHR_USE_DET_RESET  		(1<<30)			// Use DeResetTrimmed to disable Over threshold trigger during detector reset (usually Unnecessary)
//! [XSP3_TRIGGERBC_OVER_THRESHOLD_SERVO_REGISTER]

//! [XSP3_TRIGGERC_OVER_THRESHOLD_SERVO_REGISTER]
#define X3TRIG_C_SET_OVERTHR_SERVO_TRIM(x)     (((x)&0xF)<<16)
#define X3TRIG_C_GET_OVERTHR_SERVO_TRIM(x)     (((x)>>16)&0xF)
//! [XSP3_TRIGGERC_OVER_THRESHOLD_SERVO_REGISTER]

//! [XSP3_TRIGGERC_THRESHOLD_REGISTER]
#define X3TRIG_C_THRES_SET_ARM(x)			(((x)&0x3FF))
#define X3TRIG_C_THRES_SET_END(x)			(((x)&0x3FF)<<10)
#define X3TRIG_C_THRES_SET_DIFF1(x)			(((x)&0x7F)<<20)
#define X3TRIG_C_THRES_SET_AVEMODE(x) 		(((x)&3)<<27)		/* Average mode */
#define X3TRIG_C_THRES_TWO_OVER				(1<<29)
#define X3TRIG_C_THRES_ENB_NEGATIVE			(1<<30)
#define X3TRIG_C_THRES_IGNORE_NEAR_GLITCH	(1<<31)

#define X3TRIG_C_THRES_GET_ARM(x)			(((x)&0x3FF))
#define X3TRIG_C_THRES_GET_END(x)			(((x)>>10)&0x3FF)
#define X3TRIG_C_THRES_GET_DIFF1(x)			(((x)>>20)&0x7F)
#define X3TRIG_C_THRES_GET_AVEMODE(x) 		(((x)>>27)&3)
//! [XSP3_TRIGGERC_THRESHOLD_REGISTER]

//! [XSP3_TRIGGER_B_RINGING_REGISTER]
#define X3TRIG_B_RINGING_SCALE_A(x)	(((x)&0x7FF))			// 11 bit Signed scaling where 1024 would be 0.5
#define X3TRIG_B_RINGING_DELAY_A(x)	(((x)&0x1F)<<11)		// Delay coded 0..31 gives a delay of 3..34
#define X3TRIG_B_RINGING_SCALE_B(x)	(((x)&0x7FF)<<16)		// Scaling for second correction term
#define X3TRIG_B_RINGING_DELAY_B(x)	(((x)&0x1F)<<27)		// Delay for second correction tap

#define X3TRIG_B_GET_RINGING_SCALE_A(x)	(((x)&0x7FF))			// 11 bit Signed scaling where 1024 would be 0.5
#define X3TRIG_B_GET_RINGING_DELAY_A(x)	(((x)>>1)&0x1F)		// Delay coded 0..31 gives a delay of 3..34
#define X3TRIG_B_GET_RINGING_SCALE_B(x)	(((x)>>16)&0x7FF)		// Scaling for second correction term
#define X3TRIG_B_GET_RINGING_DELAY_B(x)	(((x)>>27)&0x1F)		// Delay for second correction tap

#define XSP3_TRIGB_RING_MIN_DEL 3
//! [XSP3_TRIGGER_B_RINGING_REGISTER]

//! [XSP3_TRIGGERC_AVEMODE_BITS 27-28]
#define X3TRIG_C_AVE2				0
#define X3TRIG_C_AVE4				1
#define X3TRIG_C_AVE8				2
#define X3TRIG_C_AVE16				3
//! [XSP3_TRIGGERC_AVEMODE_BITS 27-28]

#define X3TRIG_B_SCALED_THRES_OFF 0
#define X3TRIG_B_SCALED_THRES_DIV4 0x8
#define X3TRIG_B_SCALED_THRES_DIV2 0x9

#define X3TRIG_B_AVE1				0
#define X3TRIG_B_AVE2				1
#define X3TRIG_B_AVE4				2
#define X3TRIG_B_AVE8				3

#define X3DEFAULT_TRIGB_DELAY       (40+16) /* Trig B delay to use when using trigger A only */
#define X3TRIG_B_MIN_DELAY			(18+8+16+1)
#define X3TRIG_B_MAX_TRIM			31
#define X3TRIG_B_MAX_OVERTHR_DELAY	31
#define X3TRIG_B_MAX_OVERTHR_STRETCH	0x3F
#define X3TRIG_B_MAX_THRES			0x3FF


#define X3TRIG_C_MAX_THRES			0x3FF
#define X3TRIG_C_MAX_SEP1			0x7F
#define X3TRIG_C_MAX_OVERTHR_DELAY	31
#define X3TRIG_C_MAX_OVERTHR_STRETCH	0x3F
#define X3TRIG_C_MAX_TRIM			15

#define X3TRIG_B_MAX_EVENT_TIMEL		0x7F

/* XSPRESS3 In Window registers */
#define X3WINDOW_LOW_START 	    	 0
#define X3WINDOW_LOW_MASK 	0x0000ffff
#define X3WINDOW_HIGH_START 	    16 
#define X3WINDOW_HIGH_MASK 	0xffff0000
#define X3WINDOW_ALL_EVENT_MASK  0xf000f000

/* Interaction of scope mode with servo settling */
#define X3SCOPE_DELAY_TRIG      (1<<23)
#define X3SCOPE_RESTART_SERVO   (1<<24)

#define XSP3_SERVO_C_ENABLE			1

//! [XSP3_SERVO_A_REGISTER]
#define XSP3_SERVO_A_DIV(x)				((x)&7)
#define XSP3_SERVO_A_STRETCH(x)			(((x)&0x7F)<<3)
#define XSP3_SERVO_A_PWL_TIME(x)		(((x)&3)<<10)
#define XSP3_SERVO_A_PRE_GAP(x)			(((x)&0xF)<<12)
#define XSP3_SERVO_A_GRAD_ERR_LIM(x)	(((x)&0xFF)<<16)
#define XSP3_SERVO_A_MASK_GLOBAL_GLITCH	(1<<24)
#define XSP3_SERVO_A_ACCUM_ON_UPDATE	(1<<25)
#define XSP3_SERVO_A_CLEAR_ON_UPDATE	(1<<26)
#define XSP3_SERVO_A_CAL_EXCLUDE(x)  	(((x)&3)<<27)
#define XSP3_SERVO_A_FROM_CAL			(1<<29)
#define XSP3_SERVO_A_MASK_GLITCH     	(1<<30)
#define XSP3_SERVO_A_PWL_ENB         	(1<<31)

#define XSP3_SERVO_A_GET_STRETCH(x)		(((x)>>3)&0x7F)
#define XSP3_SERVO_A_GET_PWL_TIME(x)	(((x)>>10)&3)
#define XSP3_SERVO_A_GET_PRE_GAP(x)		(((x)>>12)&0xF)
//! [XSP3_SERVO_A_REGISTER]

//! [XSP3_SERVO_B_REGISTER]
#define XSP3_SERVO_B_GRAD_EST_LIM(x)	(((x)&0xFFFF)<<0)
#define XSP3_SERVO_B_ENB_GLOB_GRAD		(1<<16)
#define XSP3_SERVO_B_GLOB_ADJ_SIZE(x)	(((x)&0x3)<<17)
#define XSP3_SERVO_B_GLOB_ADJ_POSN(x)	(((x)&0x7F)<<19)
#define XSP3_SERVO_B_GLOB_ADJ_SHIFT(x)	(((x)&0x7)<<26)
#define XSP3_SERVO_B_STRETCH_TIME		(1<<29)
#define XSP3_SERVO_B_GLITCH_AS_RESET	(1<<30)
#define XSP3_SERVO_B_USE_ADC_TOP4		(1<<31)
//! [XSP3_SERVO_B_REGISTER]

//! [XSP3_SERVO_C_REGISTER]
#define XSP3_SERVO_C_ENB_SERVO			(1<<0)
#define XSP3_SERVO_C_RESTART_ON_RUN		(1<<1)
#define XSP3_SERVO_C_USE_OTD_SERVO		(1<<2)
#define XSP3_SERVO_C_SET_RESET_STRETCH(x) (((x)&0x1FF)<<3)
#define XSP3_SERVO_C_SET_DITHER_CONT(x) (((x)&0x7)<<12)
#define XSP3_SERVO_C_GET_RESET_STRETCH(x) (((x)>>3)&0x1FF)
#define XSP3_SERVO_C_GET_DITHER_CONT(x) (((x)>>12)&0x7)
#define XSP3_SERVO_C_MAX_RESET_STRETCH	0x1FF
#define XSP3_SERVO_C_MAX_DITHER_CONT	5

//! [XSP3_SERVO_C_REGISTER]

#define XSP3_SERVO_MAX_ERR_LIM		0xFF
#define XSP3_SERVO_MAX_STRETCH		0x7F



#define XSP3_CHAN_GLOB 15

//! [XSP3_REGIONS]
#define XSP3_REGION_RAM_TESTPAT		1
#define XSP3_REGION_RAM_RESET_TAIL	2
#define XSP3_REGION_RAM_QUOTIENT	3
#define XSP3_REGION_RAM_ROI			4
#define XSP3_REGION_RAM_EVENT_TAIL	5
#define XSP3_REGION_RAM_PWL_SERVO	6
#define XSP3_REGION_RAM_PILEUP_TIME	7
#define XSP3_REGION_RAM_AUX1		8
#define XSP3_REGION_RAM_SERVO_TAIL	9
#define XSP3_REGION_RAM_EVENT_LEAD	10
#define XSP3_REGION_GLOB_REG		15
//! [XSP3_REGIONS]

#define XSP3_REGION_RAM_MAX		10
#define XSP3_REGION_REGS		0

#define XSP3_REGS_SIZE			32
#define XSP3_CHAN_TP_SIZE		0
#define XSP3_RESET_TAIL_SIZE	1024
#define XSP3_QUOTIENT_SIZE		1024
#define XSP3_ROI_SIZE			4096
#define XSP3_EVENT_TAIL_SIZE	1024
#define XSP3_PWL_SERVO_SIZE		512
#define XSP3_PILEUP_TIME_SIZE	2048
#define XSP3_AUX1_SIZE			4096
#define XSP3_SERVO_TAIL_SIZE	1024
#define XSP3_EVENT_LEAD_SIZE	1024

/* Set to max BRAM size for use in save/restore */
#define XSP3_MAX_BRAM_SIZE 		4096

#define XSP3_FEV_TP_RESET		0x8000

#define XSP3_PWL_SERVO_SIZE16 2048

/* Global registers *.
 *
 */
#define XSP3_GLOB_CLOCK_CONT		0
#define XSP3_GLOB_MDIO_WRITE		1

#define XSP3_GLOB_TIMING_A			2
#define XSP3_GLOB_TIMING_FIXED		3

#define XSP3_GLOB_SCOPE_CHAN_SEL	4
#define XSP3_GLOB_SCOPE_SRC_SEL		5
#define XSP3_GLOB_SCOPE_NWORDS		6
#define XSP3_GLOB_SCOPE_ALTERNATE	7

#define XSP3_GLOB_DATA_PATH_CONT	8
// Register 9 is available now.
#define XSP3_GLOB_DATA_MUX_CONT		10
#define XSP3_NUM_GLOB_REG			11

#define XSP3_GLOB_FAN_SPEED 		31

#define XSP3_GLOB_MDIO_READ			33

#define XSP3_GLOB_LL_STATUS			32
#define XSP3_GLOB_TIMING_STATUS_A	34

//! [XSP3_GLOBAL_CLOCK]
#define XSP3_GLOB_CLK_FROM_ADC		(1<<0)			 // Enable Clock from Spartans (from ADC) for Virtex 5 (0 at reset, write 1 for normal operation after setting up ADC board Clocks)
#define XSP3_GLOB_CLK_SP_RESET		(1<<1)			 // Software reset of Spartan DCMs after setting up ADC board Clocks.
#define XSP3_GLOB_CLK_TP_ENB_SP_TOP	(1<<8)			 // Enable Test pattern from Spartans TOP
#define XSP3_GLOB_CLK_RS232_SEL		(1<<31) 		 // Select for RS232 control (temporary).
//! [XSP3_GLOBAL_CLOCK]

//! [XSP3_GLOBAL_TIMEA_BITS 0..2]
#define XSP3_GTIMA_SRC_FIXED			0		// Fixed constant time frame now replaced by Software timed but incremented time frame
#define XSP3_GTIMA_SRC_SOFTWARE			0		// Timing controlled by software, incrementing on each subsequent continue
#define XSP3_GTIMA_SRC_INTERNAL			1		// Time frame from internal timign generator (for future expansion)
#define XSP3_GTIMA_SRC_IDC				3		// Time frame incremented and rest by signals from IDC expansion connector
#define XSP3_GTIMA_SRC_TTL_VETO_ONLY	4		// Time frame incremented by TTL Input 1
#define XSP3_GTIMA_SRC_TTL_BOTH			5		// Time frame incremented by TTL Input 1 and reset to Fixed register by TTL Input 0
#define XSP3_GTIMA_SRC_LVDS_VETO_ONLY	6		// Time frame incremented by LVDS Input
#define XSP3_GTIMA_SRC_LVDS_BOTH		7		// Time frame incremented and reset by LVDS Inputs
//! [XSP3_GLOBAL_TIMEA_BITS 0..2]

//! [XSP3_GLOBAL_TIMEA_REGISTER]
#define XSP3_GLOB_TIMA_TF_SRC(x)		((x)&7)		// Sets Time frame info source see XSP3_GTIMA_SRC_*
#define XSP3_GLOB_TIMA_F0_INV			(1<<3)		// Invert Frame Zero signal polarity to make signal active low, resets time frame when sampled low by leading edge of Veto
#define XSP3_GLOB_TIMA_VETO_INV			(1<<4)		// Invert Veto signal polarity to make signal active low, counts when Veto input is low.
#define XSP3_GLOB_TIMA_ENB_SCALER		(1<<5)		// Enables scalers
#define XSP3_GLOB_TIMA_ENB_HIST			(1<<6)		// Enables histogramming
#define XSP3_GLOB_TIMA_LOOP_IO			(1<<7)		// Loop TTL_IN(0..3) to TTL_OUT(0..3) for hardware testing (only).
#define XSP3_GLOB_TIMA_NUM_SCAL_CHAN(x) (((x)&0xF)<<8)	// Sets the number of channels of scalers to be transfered to memory by the DMA per time frame.
#define XSP3_GLOB_TIMA_DEBOUNCE(x)		(((x)&0xFF)<<16)	// Set debounce time in 80 MHz cycles to ignore glitches or ringing on Frame0 or Framign signal from any source
#define XSP3_GLOB_TIMA_ALT_TTL(x)		(((x)&0xF)<<24)		// Alternate uses of the TTL Outputs (including channel in windows signals etc)
#define XSP3_GLOB_TIMA_RUN				(1<<31)		// Over Run enable signal, set after all DMA channels have been configured
#define XSP3_GLOB_TIMA_PB_RST			(1<<30)		// Resets Playback FIFO as part of clean start.
#define XSP3_GLOB_TIMA_COUNT_ENB		(1<<29)		// In software timing (XSP3_GTIMA_SRC_FIXED) mode enable counting when high. Transfers scalers on falling edge. After first frame, increments time frame on risign edge


#define XSP3_ALT_TTL_TIMING					0		//!< Place holder for possible futher timing generator.
#define XSP3_ALT_TTL_INWINDOW				0x8		//!< Output in-window signal for up to 4 channels
#define XSP3_ALT_TTL_INWINLIVE				0x9		//!< Output 2 in-window signals and 2 live signals
#define XSP3_ALT_TTL_INWINLIVETOGGLE		0xA		//!< Output 2 in-window signals and 2 live signals toggling
#define XSP3_ALT_TTL_INWINGOODLIVE			0xB		//!< Outputs in-window Allevent AllGood and LiveLevel from Chan 0
#define XSP3_ALT_TTL_INWINGOODLIVETOGGLE	0xC		//!< Outputs in-window Allevent AllGood and Live toggling from Chan 0


//! [XSP3_GLOBAL_TIMEA_REGISTER]

#define XSP3_GSCOPE_CS_ENB_SCOPE	1
#define XSP3_GSCOPE_CS_BYTE_SWAP	(1<<1)

#define XSP3_GSCOPE_CHAN_SEL(s,x)	(((x)&0xF)<<4*((s)+1))
#define XSP3_GSCOPE_CHAN_SEL_GET(s,x) (((x)>>4*((s)+1))&0xF)

#define XSP3_GSCOPE_CHAN_SEL0(x)	(((x)&0xF)<<4)
#define XSP3_GSCOPE_CHAN_SEL1(x)	(((x)&0xF)<<8)
#define XSP3_GSCOPE_CHAN_SEL2(x)	(((x)&0xF)<<12)
#define XSP3_GSCOPE_CHAN_SEL3(x)	(((x)&0xF)<<16)
#define XSP3_GSCOPE_CHAN_SEL4(x)	(((x)&0xF)<<20)
#define XSP3_GSCOPE_CHAN_SEL5(x)	(((x)&0xF)<<24)

#define XSP3_GSCOPE_SRC_SEL(s,x)	(((x)&0xF)<<4*(s))
#define XSP3_GSCOPE_SRC_SEL_GET(s,x) (((x)>>4*((s)))&0xF)

#define XSP3_GSCOPE_SRC_SEL0(x)		(((x)&0xF)<<0)
#define XSP3_GSCOPE_SRC_SEL1(x)		(((x)&0xF)<<4)
#define XSP3_GSCOPE_SRC_SEL2(x)		(((x)&0xF)<<8)
#define XSP3_GSCOPE_SRC_SEL3(x)		(((x)&0xF)<<12)
#define XSP3_GSCOPE_SRC_SEL4(x)		(((x)&0xF)<<16)
#define XSP3_GSCOPE_SRC_SEL5(x)		(((x)&0xF)<<20)

#define XSP3_GSCOPE_ALT(s,x)		(((x)&0xF)<<4*(s))

#define XSP3_GSCOPE_ALT0(x)			(((x)&0xF)<<0)
#define XSP3_GSCOPE_ALT1(x)			(((x)&0xF)<<4)
#define XSP3_GSCOPE_ALT2(x)			(((x)&0xF)<<8)
#define XSP3_GSCOPE_ALT3(x)			(((x)&0xF)<<12)
#define XSP3_GSCOPE_ALT4(x)			(((x)&0xF)<<16)
#define XSP3_GSCOPE_ALT5(x)			(((x)&0xF)<<20)
#define XSP3_GSCOPE_ALTERNATE_GET(s,x) (((x)>>4*((s)))&0xF)

#define XSP3_SCOPE_NUM_STREAMS 6
/* Stream 0 is usually used for the digital bits associated with the other 5 streams */
#define XSP3_SCOPE_SEL0_INP			0	// ADC Data
#define XSP3_SCOPE_SEL0_DIGITAL		1	// Digital data associated with analogue on other 5 streams
#define XSP3_SCOPE_SEL0_ALL_RESETS		2	// Digital data from All channel resets
#define XSP3_SCOPE_SEL0_RESET_DET_RESETS	3	// Digital data from All channel Reset Detector Det resets

/* Streams 1..3 are 4 input mux . Input, TriggerB and Servo are available in all 5 streams as these are used for the whole detector auto calibration */
/* Output is available in stream 1..3 for use to search for bad events when debugging 1 or 2 channels */
#define XSP3_SCOPE_SEL123_INP			0
#define XSP3_SCOPE_SEL123_TRIG_B_OUT	1
#define XSP3_SCOPE_SEL123_SERVO_OUT		2
#define XSP3_SCOPE_SEL123_DIG_OUT		3

/* Streams 4 & 5 are 8 input mux with more detail */
#define XSP3_SCOPE_SEL45_INP			0
#define XSP3_SCOPE_SEL45_TRIG_B_OUT		1
#define XSP3_SCOPE_SEL45_SERVO_OUT		2
#define XSP3_SCOPE_SEL45_GLITCH			3
#define XSP3_SCOPE_SEL45_TRIG_B_DIFF1	4
#define XSP3_SCOPE_SEL45_TRIG_B_DIFF2	5
#define XSP3_SCOPE_SEL45_SERVO_GRAD_ERR	6
#define XSP3_SCOPE_SEL45_SERVO_GRAD_EST	7
#define XSP3_SCOPE_SEL45_RESET_DET		8
#define XSP3_SCOPE_SEL45_TRIG_C_DIFF1	9

#define XSP3_GSCOPE_CS_HIST_TO_DMA	(1<<2)

#define XSP3_DPC_HIST_TO_DMA		(1<<0)		// Send Hist dat to DMA rather than directly to 10 G Core
#define XSP3_DPC_ENB_10G_RX			(1<<1)		// Enable 10G Data to DMA channel
#define XSP3_DPC_ENB_10G_RX_ACKS	(1<<2)		// Enable Hardware Acknowledge packets fo 10G RX data and set multiplexer to connect 10G TX to ACKs.
#define XSP3_DPC_SEL_10G_TX_DMA		(1<<3)		// Select 10G Tx data path from DMA (usually for scope mode output).
#define XSP3_DPC_FARM_MODE			(1<<4)		// Enable Farm Mode (1 word = Chan = select PC/Port in Hist to LL direct to 10G

#define XSP3_DPC_RX_RESET			(1<<30)		// Reset all the RX data paths Excluding the xaui block
#define XSP3_DPC_TX_RESET			(1<<31)		// Reset all the TX data paths Excluding the xaui block


#define XSP3_GLOB_LL_STAT_PLAYBACK_OR	(1<<0)
#define XSP3_GLOB_LL_STAT_SCOPE_OR		(1<<1)
#define XSP3_GLOB_LL_STAT_SCALERS_OR	(1<<2)

#define XLLDMA_SR_IRQ_ON_END_MASK   0x00000040 /**< IRQ on end has occurred */
#define XLLDMA_SR_STOP_ON_END_MASK  0x00000020 /**< Stop on end has occurred */
#define XLLDMA_SR_COMPLETED_MASK    0x00000010 /**< BD completed */
#define XLLDMA_SR_SOP_MASK          0x00000008 /**< Current BD has SOP set */
#define XLLDMA_SR_EOP_MASK          0x00000004 /**< Current BD has EOP set */
#define XLLDMA_SR_ENGINE_BUSY_MASK  0x00000002 /**< Channel is busy */


//! [XSP3_GLOB_ADC_DATA_MUX_CONT]
#define XSP3_GLOB_ADC_MUX(adc)	(((adc)&0xF)<<0) // ADC Channel Mux selects any 1 channel which can be selected by any or all channels using XSP3_CC_SEL_DATA_MUX_DATA
												 // This allows different processing options to be trialled on the same data stream.
//! [XSP3_GLOB_ADC_DATA_MUX_CONT]

//! [XSP3_GLOBAL_TIME_STATUS_A]
#define XSP3_GLOB_TSTAT_A_TTL_IN(x)		(((x)>>28)&0xF)	// Read the 4 TTL LEMO inputs input 0 as bit 0, input 1 as 1... 

//! [XSP3_GLOBAL_TIME_STATUS_A]

extern int xsp3_bram_size[XSP3_REGION_RAM_MAX+1];
extern int xsp3_bram_width[XSP3_REGION_RAM_MAX+1];
extern const char *xsp3_bram_name[XSP3_REGION_RAM_MAX+1];

//! [XSP3_RUN_FLAGS]
#define XSP3_RUN_FLAGS_PLAYBACK 1
#define XSP3_RUN_FLAGS_SCOPE 	2
#define XSP3_RUN_FLAGS_SCALERS 	4
#define XSP3_RUN_FLAGS_HIST 	8
//! [XSP3_RUN_FLAGS]

#define XSP3_SPI_S3_READ_WRITE_MASK 0xFFFFFF
#define XSP3_SPI_SS_SPARTAN		5
#define XSP3_SPI_SS_CLK0		0
#define XSP3_SPI_SS_CLK1		1
#define XSP3_SPI_SS_CLK2		2
#define XSP3_SPI_SS_CLK3		3

//! [XSP3_SPI_REGISTER]
#define XSP3_SPI_CLK_GOE0			(1<<0)	
#define XSP3_SPI_CLK_GOE1			(1<<1)
#define XSP3_SPI_CLK_GOE2			(1<<2)
#define XSP3_SPI_CLK_GOE3			(1<<3)
#define XSP3_SPI_CLK_NSYNC0			(1<<4)
#define XSP3_SPI_CLK_NSYNC1			(1<<5)
#define XSP3_SPI_CLK_NSYNC2			(1<<6)
#define XSP3_SPI_CLK_NSYNC3			(1<<7)
#define XSP3_SPI_DITHER				(1<<8)
#define XSP3_SPI_PSU(x)				(((x)&0x7F)<<9)
#define XSP3_SPI_TP_TYPE(x)			(((x)&0x3)<<21)
#define XSP3_SPI_TP_ENB				(1<<23)
#define XSP3_SPI_CLK_LD2			(1<<24)
#define XSP3_SPI_CLK_LD3			(1<<25)
#define XSP3_SPI_CLK_DIS_OVER_TEMP	(1<<9)
#define XSP3_SPI_CLK_SHUTDOWN0		(1<<10)
#define XSP3_SPI_CLK_SHUTDOWN123	(1<<11)
#define XSP3_SPI_CLK_SHUTDOWN4		(1<<12)
#define XSP3_SPI_CLK_SHUTDOWN5678	(1<<13)
//! [XSP3_SPI_REGISTER]

#define XSP3_I2C_BUS_ADC	3
#define XSP3_LM75_ADDR(dev) (0x9<<3|((dev)&7))

//! [XSP3_CLOCK_SRC]
#define XSP3_CLK_SRC_INT		0		// channel processing clock comes from fpga processor (testing only)
#define XSP3_CLK_SRC_XTAL		1		// adc and channel processing clock from crystal on the ADC board (normal single board or master operation).
#define XSP3_CLK_SRC_EXT		2		// adc and channel processing clock from lemo clock connector on ADC board (slave boards)
#define XSP3_CLK_SRC_FPGA		3		// not implemented, for future expansion
//! [XSP3_CLOCK_SRC]

//! [XSP3_CLOCK_FLAGS]
#define XSP3_CLK_FLAGS_MASTER		(1<<0)		// this clock generate clocks for other boards in the system
#define XSP3_CLK_FLAGS_DITHER		(1<<1)		// enables dither within the ADC
#define XSP3_CLK_FLAGS_STAGE1_ONLY	(1<<2)		// performs stage of the lmk 03200 setup, does not enable zero delay mode
#define XSP3_CLK_FLAGS_NO_CHECK		(1<<3)		// dont check for lock detect from lmk 03200
#define XSP3_CLK_FLAGS_TP_ENB		(1<<4)		// enable test pattern from spartans
#define XSP3_CLK_FLAGS_DIS_OVER_TEMP (1<<5)		// Disable Over temperature protection on ADC Board
#define XSP3_CLK_FLAGS_SHUTDOWN0 	(1<<6)		// Shutdown ADC channel 0
#define XSP3_CLK_FLAGS_SHUTDOWN123 	(1<<7)		// Shutdown ADC channels 123
#define XSP3_CLK_FLAGS_SHUTDOWN4 	(1<<8)		// Shutdown ADC channel 4 (middle (unused?))
#define XSP3_CLK_FLAGS_SHUTDOWN5678 (1<<9)		// Shutdown ADC channel5678 last 4

//! [XSP3_CLOCK_FLAGS]

/* Now removed from format control */
//#define XSP3_FORMAT_DISABLE_TFG		2	/* Write 1 here to disable TFG input and just use software controlled timing 	*/
//#define XSP3_FORMAT_DISABLE_SCALER		4	/* Disable scaler so top histogramming locations are not overwritten.		 	*/
//#define XSP3_FORMAT_DISABLE_HIST		8   /* Disable all histogram data and use all memory for many scalers.				*/

#define XSP3_FORMAT_AUX1_MODE(x)	(((x)&0xF)<<4)
#define XSP3_FORMAT_AUX1_THRESH(x)	(((x)&0x3FF)<<8)
#define XSP3_FORMAT_NBITS_ENG(x)	(((x)&0xF)<<21)
#define XSP3_FORMAT_NBITS_ADC(x)	(((x)&0x7)<<25)
#define XSP3_FORMAT_AUX_MODE(x)		(((x)&0x7)<<28)
#define XSP3_FORMAT_PILEUP_REJECT	(1<<31)

#define XSP3_FORMAT_NBITS_AUX0	0
#define XSP3_FORMAT_NBITS_AUX4	1
#define XSP3_FORMAT_NBITS_AUX6	2
#define XSP3_FORMAT_NBITS_AUX8	3
#define XSP3_FORMAT_NBITS_AUX10	4
#define XSP3_FORMAT_NBITS_AUX12	5
#define XSP3_FORMAT_NBITS_AUX14	6
#define XSP3_FORMAT_NBITS_AUX16	7

//! [XSP3_AUX2_FLAGS]
#define XSP3_FORMAT_AUX2_ADC					0
#define XSP3_FORMAT_AUX2_WIDTH					1
#define XSP3_FORMAT_AUX2_RST_START_ADC			2
#define XSP3_FORMAT_AUX2_NEB_RST_ADC			3
#define XSP3_FORMAT_AUX2_NEB_RST_RST_START_ADC	4
#define XSP3_FORMAT_AUX2_TIME_FROM_RST			5
#define XSP3_FORMAT_AUX2_NEB_RST_TIME_FROM_RST	6
#define XSP3_FORMAT_AUX2_NEB_RST_TIME_FROM_RSTX8 7
//! [XSP3_AUX2_FLAGS]

#define XSP3_FORMAT_AUX1_NONE		0		/* No resolution grades 									*/
#define XSP3_FORMAT_AUX1_MINDIV8	1		/* Normal Resolution grade mode of shorter of two runs / 8 	*/
#define XSP3_FORMAT_AUX1_THRES		2		/* 2 Res grades using threshold 						 	*/
#define XSP3_FORMAT_AUX1_LOG		3		/* Approx logarithmic grade 									*/
#define XSP3_FORMAT_AUX1_TOP		4		/* All 7 or 10 bits of top count							*/
#define XSP3_FORMAT_AUX1_BOT		5		/* All 7 or 10 bits of Bottom Count							*/
#define XSP3_FORMAT_AUX1_MIN		6		/* All 7 or 10 bits of Min of top and bottom				*/
#define XSP3_FORMAT_AUX1_PILEUP		7
#define XSP3_FORMAT_AUX1_LUT_SETUP	8		/* 12 bits of res grade to learn setup for LO/LUT mode 		*/
#define XSP3_FORMAT_AUX1_LUT_THRES	9		/* 1 bit thresholded using the LUT LSB						*/

#define XSP3_FORMAT_AUX1_GOOD_GRADE	15		/* 0 bits, but Masks MCA (and scalers) for events with min < thres	*/

#define XSP3_FORMAT_GET_AUX1_MODE(x)	(((x)>>4)&0xF)
#define XSP3_FORMAT_GET_AUX1_THRES(x)	(((x)>>8)&0x3FF)
#define XSP3_FORMAT_GET_NBITS_ENG_LOST(x)	(((x)>>21)&0xF)
#define XSP3_FORMAT_GET_NBITS_ADC(x)	(((x)>>25)&0x7)

#define XSP3_MIN_BITS_ENG  1
#define XSP3_MAX_BITS_ENG  12

#define XSP3_DTC_OMIT_CHANNEL (1<<0)
#define XSP3_DTC_USE_GOOD_EVENT (1<<1)

#define XSP3_SCALER_TIME			0
#define XSP3_SCALER_RESETTICKS		1
#define XSP3_SCALER_RESETCOUNT		2
#define XSP3_SCALER_ALLEVENT		3
#define XSP3_SCALER_ALLGOOD			4
#define XSP3_SCALER_INWINDOW0		5
#define XSP3_SCALER_INWINDOW1		6
#define XSP3_SCALER_PILEUP			7

#define XSP3_MDIO_DIRECT	0
#define XSP3_MDIO_AUX		1


/*
 * fan_control.h
 *
 *  Created on: 20 Feb 2013
 *      Author: wih73
 */
typedef struct _fan_log {
	int16_t temp; 	// Signed int in 0.5 deg C units
	int16_t speed; // 0 = off, 4000 = full speed
} FanLog;
#define XSP3_FAN_LOG_POINTS 10000
#define XSP3_FAN_MODE_OFF				0
#define XSP3_FAN_MODE_MONITOR_LOOP 		1
#define XSP3_FAN_MODE_MONITOR_ONESHOT 	2
#define XSP3_FAN_MODE_CONTROL 			3

#define XSP3_FAN_CONT_NUM_AVERAGE 10
#define XSP3_FAN_CONT_FRAC_PRECISION 10

typedef struct _fan_cont
{
	int32_t mode;
	int32_t start_run;
	int32_t cur_temp[4];
	int32_t cur_max;
	int32_t target, p_const, i_const;
	int32_t cur_point;
	FanLog log[XSP3_FAN_LOG_POINTS];
} FanControl;

#define XSP3_FAN_OFFSET_MODE 		0
#define XSP3_FAN_OFFSET_START 		1
#define XSP3_FAN_OFFSET_TEMP 		2
#define XSP3_FAN_OFFSET_MAX 		6
#define XSP3_FAN_OFFSET_SET_POINT 	7
#define XSP3_FAN_OFFSET_P_CONST 	8
#define XSP3_FAN_OFFSET_I_CONST 	9
#define XSP3_FAN_OFFSET_CUR_POINT 	10
#define XSP3_FAN_OFFSET_LOG			11

#endif /* XSPRESS3_H_ */

