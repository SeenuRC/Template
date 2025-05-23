/**
* \file dp_SBand_Radar_signals.h
* \brief This file contains the signal ID details for DAPAR-ST Checkout System
*
* author Sathishkumar K
* \date  27 February, 2020
*
* \version   1.00
*
* \copyright Copyright (C) 2020 Data Patterns (India) Pvt. Ltd. / Indus Teqsite Pvt. Ltd. \n
* All Rights Reserved.\n
* Address: Plot # H9, Fourth Main Road, SIPCOT IT Park, Siruseri, \n
* Off Rajiv Gandhi Salai (OMR), Pudupakkam P.O., \n
* Chennai-603103 | India\n
* Website : http://www.datapatternsindia.com/\n
*
*/

#ifndef DP_SBAND_RADAR_SIGNALS_H
#define DP_SBAND_RADAR_SIGNALS_H

/* Maximum boards */
#define SBDRDR_IP3039_MAX_BRDS		2
#define SBDRDR_XMC5775_MAX_BRDS		1


#define SID_GET_IP3039_DIO_SIG(ucBoardNo, ucChnNo) 		(BRD_IP3039_DIO		+ (ucBoardNo << 8) + ucChnNo) 		/* get the Signal value of IP3039_DIO from Board No. and Channel No. */
#define SID_GET_XMC5775_LVTTL_SIG(ucBoardNo, ucChnNo) 	(BRD_XMC5775_LVTTL	+ (ucBoardNo << 8) + ucChnNo) 		/* get the Signal value of XMC5775_LVTTL from Board No. and Channel No. */
#define SID_GET_XMC5775_LVDS_SIG(ucBoardNo, ucChnNo) 	(BRD_XMC5775_LVDS	+ (ucBoardNo << 8) + ucChnNo) 		/* get the Signal value of XMC5775_LVDS from Board No. and Channel No. */
#define SID_GET_XMC5775_393_LVDS_SIG(ucBoardNo, ucChnNo) 	(BRD_XMC5775_LVDS	+ (ucBoardNo << 8) + ucChnNo) 		/* get the Signal value of XMC5775_LVDS from Board No. and Channel No. */
#define SID_GET_IP3084_DIO_SIG(ucBoardNo, ucChnNo)      (BRD_XMC5775_LVDS	+ (ucBoardNo << 8) + ucChnNo) 		/* get the Signal value of XMC5775_LVDS from Board No. and Channel No. */

#define SID_GET_CHANNEL(usSignalId) 	(usSignalId & 0x00FF) 		/* get the Channel number from the signal ID (bit 0 to 7) */
#define SID_GET_BOARD_NUM(usSignalId) 	((usSignalId >> 8) & 0x000F) 	/* get the board number from the signal ID (bit 8 to 11)*/
#define SID_GET_BOARD_ID(usSignalId) 	((usSignalId >> 12) & 0x000F) 	/* get the board ID from the signal ID (12 to 15)*/

/* To define unique signal ID based on Channel number, board number & board ID */
/* In signal ID the 8th to 11th bits are used as board number for corresponding channel */
#define SID_BOARD_1		0x0100 	/* 1st board */
#define SID_BOARD_2		0x0200 	/* 2nd board */
#define SID_BOARD_3		0x0300	/* 3rd board */
#define SID_BOARD_4		0x0400	/* 4th board */
#define SID_BOARD_5		0x0500	/* 5th board */
#define SID_BOARD_6		0x0600 	/* 6th board */


/* In signal ID the bit 12 to 15 are used as board ID */
#define BRD_IP3039_DIO		0x1000   /* DP-IP-3039 - DIP */
#define BRD_XMC5775_LVTTL	0x2000	 /* DP-XMC-5775 - LVTTL */
#define BRD_XMC5775_LVDS	0x3000	 /* DP-XMC-5775 - LVDS */


/* ############################################################################################################## */

/*********  Signal ID  ***********************  Channel Mapping  **************  Description  *************/

/* DP-IP-3039 - Digital Input & Output */

#define SG_DIO_B1_CH1			( BRD_IP3039_DIO + SID_BOARD_1 + 1 )		/* DP-IP-3039 Board 1, Channel 1 */
#define SG_DIO_B1_CH2			( BRD_IP3039_DIO + SID_BOARD_1 + 2 )		/* DP-IP-3039 Board 1, Channel 2 */
#define SG_DIO_B1_CH3			( BRD_IP3039_DIO + SID_BOARD_1 + 3 )		/* DP-IP-3039 Board 1, Channel 3 */
#define SG_DIO_B1_CH4			( BRD_IP3039_DIO + SID_BOARD_1 + 4 )		/* DP-IP-3039 Board 1, Channel 4 */
#define SG_DIO_B1_CH5			( BRD_IP3039_DIO + SID_BOARD_1 + 5 )		/* DP-IP-3039 Board 1, Channel 5 */
#define SG_DIO_B1_CH6			( BRD_IP3039_DIO + SID_BOARD_1 + 6 )		/* DP-IP-3039 Board 1, Channel 6 */
#define SG_DIO_B1_CH7			( BRD_IP3039_DIO + SID_BOARD_1 + 7 )		/* DP-IP-3039 Board 1, Channel 7 */
#define SG_DIO_B1_CH8			( BRD_IP3039_DIO + SID_BOARD_1 + 8 )		/* DP-IP-3039 Board 1, Channel 8 */
#define SG_DIO_B1_CH9			( BRD_IP3039_DIO + SID_BOARD_1 + 9 )		/* DP-IP-3039 Board 1, Channel 9 */
#define SG_DIO_B1_CH10			( BRD_IP3039_DIO + SID_BOARD_1 + 10 )		/* DP-IP-3039 Board 1, Channel 10 */
#define SG_DIO_B1_CH11			( BRD_IP3039_DIO + SID_BOARD_1 + 11 )		/* DP-IP-3039 Board 1, Channel 11 */
#define SG_DIO_B1_CH12			( BRD_IP3039_DIO + SID_BOARD_1 + 12 )		/* DP-IP-3039 Board 1, Channel 12 */
#define SG_DIO_B1_CH13			( BRD_IP3039_DIO + SID_BOARD_1 + 13 ) 		/* DP-IP-3039 Board 1, Channel 13 */
#define SG_DIO_B1_CH14			( BRD_IP3039_DIO + SID_BOARD_1 + 14 )		/* DP-IP-3039 Board 1, Channel 14 */
#define SG_DIO_B1_CH15			( BRD_IP3039_DIO + SID_BOARD_1 + 15 ) 		/* DP-IP-3039 Board 1, Channel 15 */
#define SG_DIO_B1_CH16			( BRD_IP3039_DIO + SID_BOARD_1 + 16 )		/* DP-IP-3039 Board 1, Channel 16 */
#define SG_DIO_B1_CH17			( BRD_IP3039_DIO + SID_BOARD_1 + 17 ) 		/* DP-IP-3039 Board 1, Channel 17 */
#define SG_DIO_B1_CH18			( BRD_IP3039_DIO + SID_BOARD_1 + 18 ) 		/* DP-IP-3039 Board 1, Channel 18 */
#define SG_DIO_B1_CH19			( BRD_IP3039_DIO + SID_BOARD_1 + 19 )		/* DP-IP-3039 Board 1, Channel 19 */
#define SG_DIO_B1_CH20			( BRD_IP3039_DIO + SID_BOARD_1 + 20 )		/* DP-IP-3039 Board 1, Channel 20 */
#define SG_DIO_B1_CH21			( BRD_IP3039_DIO + SID_BOARD_1 + 21 )		/* DP-IP-3039 Board 1, Channel 21 */
#define SG_DIO_B1_CH22			( BRD_IP3039_DIO + SID_BOARD_1 + 22 )		/* DP-IP-3039 Board 1, Channel 22 */
#define SG_DIO_B1_CH23			( BRD_IP3039_DIO + SID_BOARD_1 + 23 )		/* DP-IP-3039 Board 1, Channel 23 */
#define SG_DIO_B1_CH24			( BRD_IP3039_DIO + SID_BOARD_1 + 24 )		/* DP-IP-3039 Board 1, Channel 24 */
#define SG_DIO_B1_CH25			( BRD_IP3039_DIO + SID_BOARD_1 + 25 )		/* DP-IP-3039 Board 1, Channel 25 */
#define SG_DIO_B1_CH26			( BRD_IP3039_DIO + SID_BOARD_1 + 26 )		/* DP-IP-3039 Board 1, Channel 26 */
#define SG_DIO_B1_CH27			( BRD_IP3039_DIO + SID_BOARD_1 + 27 )		/* DP-IP-3039 Board 1, Channel 27 */
#define SG_DIO_B1_CH28			( BRD_IP3039_DIO + SID_BOARD_1 + 28 )		/* DP-IP-3039 Board 1, Channel 28 */
#define SG_DIO_B1_CH29			( BRD_IP3039_DIO + SID_BOARD_1 + 29 )		/* DP-IP-3039 Board 1, Channel 29 */
#define SG_DIO_B1_CH30			( BRD_IP3039_DIO + SID_BOARD_1 + 30 )		/* DP-IP-3039 Board 1, Channel 30 */
#define SG_DIO_B1_CH31			( BRD_IP3039_DIO + SID_BOARD_1 + 31 )		/* DP-IP-3039 Board 1, Channel 31 */
#define SG_DIO_B1_CH32			( BRD_IP3039_DIO + SID_BOARD_1 + 32 )		/* DP-IP-3039 Board 1, Channel 32 */
#define SG_DIO_B1_CH33			( BRD_IP3039_DIO + SID_BOARD_1 + 33 )		/* DP-IP-3039 Board 1, Channel 33 */
#define SG_DIO_B1_CH34			( BRD_IP3039_DIO + SID_BOARD_1 + 34 )		/* DP-IP-3039 Board 1, Channel 34 */
#define SG_DIO_B1_CH35			( BRD_IP3039_DIO + SID_BOARD_1 + 35 )		/* DP-IP-3039 Board 1, Channel 35 */
#define SG_DIO_B1_CH36			( BRD_IP3039_DIO + SID_BOARD_1 + 36 )		/* DP-IP-3039 Board 1, Channel 36 */
#define SG_DIO_B1_CH37			( BRD_IP3039_DIO + SID_BOARD_1 + 37 )		/* DP-IP-3039 Board 1, Channel 37 */
#define SG_DIO_B1_CH38			( BRD_IP3039_DIO + SID_BOARD_1 + 38 )		/* DP-IP-3039 Board 1, Channel 38 */
#define SG_DIO_B1_CH39			( BRD_IP3039_DIO + SID_BOARD_1 + 39 )		/* DP-IP-3039 Board 1, Channel 39 */
#define SG_DIO_B1_CH40			( BRD_IP3039_DIO + SID_BOARD_1 + 40 )		/* DP-IP-3039 Board 1, Channel 40 */
#define SG_DIO_B1_CH41			( BRD_IP3039_DIO + SID_BOARD_1 + 41 )		/* DP-IP-3039 Board 1, Channel 41 */
#define SG_DIO_B1_CH42			( BRD_IP3039_DIO + SID_BOARD_1 + 42 )		/* DP-IP-3039 Board 1, Channel 42 */
#define SG_DIO_B1_CH43			( BRD_IP3039_DIO + SID_BOARD_1 + 43 )		/* DP-IP-3039 Board 1, Channel 43 */
#define SG_DIO_B1_CH44			( BRD_IP3039_DIO + SID_BOARD_1 + 44 )		/* DP-IP-3039 Board 1, Channel 44 */
#define SG_DIO_B1_CH45			( BRD_IP3039_DIO + SID_BOARD_1 + 45 )		/* DP-IP-3039 Board 1, Channel 45 */
#define SG_DIO_B1_CH46			( BRD_IP3039_DIO + SID_BOARD_1 + 46 )		/* DP-IP-3039 Board 1, Channel 46 */
#define SG_DIO_B1_CH47			( BRD_IP3039_DIO + SID_BOARD_1 + 47 )		/* DP-IP-3039 Board 1, Channel 47 */
#define SG_DIO_B1_CH48			( BRD_IP3039_DIO + SID_BOARD_1 + 48 )		/* DP-IP-3039 Board 1, Channel 48 */

#define SG_DIO_B2_CH1			( BRD_IP3039_DIO + SID_BOARD_2 + 1 )		/* DP-IP-3039 Board 2, Channel 1 */
#define SG_DIO_B2_CH2			( BRD_IP3039_DIO + SID_BOARD_2 + 2 )		/* DP-IP-3039 Board 2, Channel 2 */
#define SG_DIO_B2_CH3			( BRD_IP3039_DIO + SID_BOARD_2 + 3 )		/* DP-IP-3039 Board 2, Channel 3 */
#define SG_DIO_B2_CH4			( BRD_IP3039_DIO + SID_BOARD_2 + 4 )		/* DP-IP-3039 Board 2, Channel 4 */
#define SG_DIO_B2_CH5			( BRD_IP3039_DIO + SID_BOARD_2 + 5 )		/* DP-IP-3039 Board 2, Channel 5 */
#define SG_DIO_B2_CH6			( BRD_IP3039_DIO + SID_BOARD_2 + 6 ) 		/* DP-IP-3039 Board 2, Channel 6 */
#define SG_DIO_B2_CH7			( BRD_IP3039_DIO + SID_BOARD_2 + 7 ) 		/* DP-IP-3039 Board 2, Channel 7 */
#define SG_DIO_B2_CH8			( BRD_IP3039_DIO + SID_BOARD_2 + 8 ) 		/* DP-IP-3039 Board 2, Channel 8 */
#define SG_DIO_B2_CH9			( BRD_IP3039_DIO + SID_BOARD_2 + 9 ) 		/* DP-IP-3039 Board 2, Channel 9 */
#define SG_DIO_B2_CH10			( BRD_IP3039_DIO + SID_BOARD_2 + 10 )		/* DP-IP-3039 Board 2, Channel 10 */
#define SG_DIO_B2_CH11			( BRD_IP3039_DIO + SID_BOARD_2 + 11 )		/* DP-IP-3039 Board 2, Channel 11 */
#define SG_DIO_B2_CH12			( BRD_IP3039_DIO + SID_BOARD_2 + 12 )		/* DP-IP-3039 Board 2, Channel 12 */
#define SG_DIO_B2_CH13			( BRD_IP3039_DIO + SID_BOARD_2 + 13 )		/* DP-IP-3039 Board 2, Channel 13 */
#define SG_DIO_B2_CH14			( BRD_IP3039_DIO + SID_BOARD_2 + 14 )		/* DP-IP-3039 Board 2, Channel 14 */
#define SG_DIO_B2_CH15			( BRD_IP3039_DIO + SID_BOARD_2 + 15 )		/* DP-IP-3039 Board 2, Channel 15 */
#define SG_DIO_B2_CH16			( BRD_IP3039_DIO + SID_BOARD_2 + 16 )		/* DP-IP-3039 Board 2, Channel 16 */
#define SG_DIO_B2_CH17			( BRD_IP3039_DIO + SID_BOARD_2 + 17 )		/* DP-IP-3039 Board 2, Channel 17 */
#define SG_DIO_B2_CH18			( BRD_IP3039_DIO + SID_BOARD_2 + 18 )		/* DP-IP-3039 Board 2, Channel 18 */
#define SG_DIO_B2_CH19			( BRD_IP3039_DIO + SID_BOARD_2 + 19 )		/* DP-IP-3039 Board 2, Channel 19 */
#define SG_DIO_B2_CH20			( BRD_IP3039_DIO + SID_BOARD_2 + 20 )		/* DP-IP-3039 Board 2, Channel 20 */
#define SG_DIO_B2_CH21			( BRD_IP3039_DIO + SID_BOARD_2 + 21 )		/* DP-IP-3039 Board 2, Channel 21 */
#define SG_DIO_B2_CH22			( BRD_IP3039_DIO + SID_BOARD_2 + 22 )		/* DP-IP-3039 Board 2, Channel 22 */
#define SG_DIO_B2_CH23			( BRD_IP3039_DIO + SID_BOARD_2 + 23 )		/* DP-IP-3039 Board 2, Channel 23 */
#define SG_DIO_B2_CH24			( BRD_IP3039_DIO + SID_BOARD_2 + 24 )		/* DP-IP-3039 Board 2, Channel 24 */
#define SG_DIO_B2_CH25			( BRD_IP3039_DIO + SID_BOARD_2 + 25 )		/* DP-IP-3039 Board 2, Channel 25 */
#define SG_DIO_B2_CH26			( BRD_IP3039_DIO + SID_BOARD_2 + 26 )		/* DP-IP-3039 Board 2, Channel 26 */
#define SG_DIO_B2_CH27			( BRD_IP3039_DIO + SID_BOARD_2 + 27 )		/* DP-IP-3039 Board 2, Channel 27 */
#define SG_DIO_B2_CH28			( BRD_IP3039_DIO + SID_BOARD_2 + 28 )		/* DP-IP-3039 Board 2, Channel 28 */
#define SG_DIO_B2_CH29			( BRD_IP3039_DIO + SID_BOARD_2 + 29 )		/* DP-IP-3039 Board 2, Channel 29 */
#define SG_DIO_B2_CH30			( BRD_IP3039_DIO + SID_BOARD_2 + 30 )		/* DP-IP-3039 Board 2, Channel 30 */
#define SG_DIO_B2_CH31			( BRD_IP3039_DIO + SID_BOARD_2 + 31 )		/* DP-IP-3039 Board 2, Channel 31 */
#define SG_DIO_B2_CH32			( BRD_IP3039_DIO + SID_BOARD_2 + 32 )		/* DP-IP-3039 Board 2, Channel 32 */
#define SG_DIO_B2_CH33			( BRD_IP3039_DIO + SID_BOARD_2 + 33 ) 		/* DP-IP-3039 Board 2, Channel 33 */
#define SG_DIO_B2_CH34			( BRD_IP3039_DIO + SID_BOARD_2 + 34 )		/* DP-IP-3039 Board 2, Channel 34 */
#define SG_DIO_B2_CH35			( BRD_IP3039_DIO + SID_BOARD_2 + 35 )		/* DP-IP-3039 Board 2, Channel 35 */
#define SG_DIO_B2_CH36			( BRD_IP3039_DIO + SID_BOARD_2 + 36 )		/* DP-IP-3039 Board 2, Channel 36 */
#define SG_DIO_B2_CH37			( BRD_IP3039_DIO + SID_BOARD_2 + 37 )		/* DP-IP-3039 Board 2, Channel 37 */
#define SG_DIO_B2_CH38			( BRD_IP3039_DIO + SID_BOARD_2 + 38 )		/* DP-IP-3039 Board 2, Channel 38 */
#define SG_DIO_B2_CH39			( BRD_IP3039_DIO + SID_BOARD_2 + 39 )		/* DP-IP-3039 Board 2, Channel 39 */
#define SG_DIO_B2_CH40			( BRD_IP3039_DIO + SID_BOARD_2 + 40 )		/* DP-IP-3039 Board 2, Channel 40 */
#define SG_DIO_B2_CH41			( BRD_IP3039_DIO + SID_BOARD_2 + 41 )		/* DP-IP-3039 Board 2, Channel 41 */
#define SG_DIO_B2_CH42			( BRD_IP3039_DIO + SID_BOARD_2 + 42 ) 		/* DP-IP-3039 Board 2, Channel 42 */
#define SG_DIO_B2_CH43			( BRD_IP3039_DIO + SID_BOARD_2 + 43 )		/* DP-IP-3039 Board 2, Channel 43 */
#define SG_DIO_B2_CH44			( BRD_IP3039_DIO + SID_BOARD_2 + 44 )		/* DP-IP-3039 Board 2, Channel 44 */
#define SG_DIO_B2_CH45			( BRD_IP3039_DIO + SID_BOARD_2 + 45 )		/* DP-IP-3039 Board 2, Channel 45 */
#define SG_DIO_B2_CH46			( BRD_IP3039_DIO + SID_BOARD_2 + 46 )		/* DP-IP-3039 Board 2, Channel 46 */
#define SG_DIO_B2_CH47			( BRD_IP3039_DIO + SID_BOARD_2 + 47 )		/* DP-IP-3039 Board 2, Channel 47 */
#define SG_DIO_B2_CH48			( BRD_IP3039_DIO + SID_BOARD_2 + 48 )		/* DP-IP-3039 Board 2, Channel 48 */


/* DP-XMC-5775 - LVTTL Input & Output*/

#define SG_LVTTL_IO_B1_1			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 1 )		/* DP-XMC-5775 Board 1, channel 1 */
#define SG_LVTTL_IO_B1_2			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 2 )  		/* DP-XMC-5775 Board 1, channel 2 */
#define SG_LVTTL_IO_B1_3			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 3 )		/* DP-XMC-5775 Board 1, channel 3 */
#define SG_LVTTL_IO_B1_4			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 4 )		/* DP-XMC-5775 Board 1, channel 4 */
#define SG_LVTTL_IO_B1_5			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 5 )		/* DP-XMC-5775 Board 1, channel 5 */
#define SG_LVTTL_IO_B1_6			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 6 )		/* DP-XMC-5775 Board 1, channel 6 */
#define SG_LVTTL_IO_B1_7			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 7 )		/* DP-XMC-5775 Board 1, channel 7 */
#define SG_LVTTL_IO_B1_8			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 8 )		/* DP-XMC-5775 Board 1, channel 8 */
#define SG_LVTTL_IO_B1_9			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 9 )		/* DP-XMC-5775 Board 1, channel 9 */
#define SG_LVTTL_IO_B1_10			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 10 )		/* DP-XMC-5775 Board 1, channel 10 */
#define SG_LVTTL_IO_B1_11			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 11 )		/* DP-XMC-5775 Board 1, channel 11 */
#define SG_LVTTL_IO_B1_12			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 12 )		/* DP-XMC-5775 Board 1, channel 12 */
#define SG_LVTTL_IO_B1_13			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 13 )		/* DP-XMC-5775 Board 1, channel 13 */
#define SG_LVTTL_IO_B1_14			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 14 )		/* DP-XMC-5775 Board 1, channel 14 */
#define SG_LVTTL_IO_B1_15			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 15 )		/* DP-XMC-5775 Board 1, channel 15 */
#define SG_LVTTL_IO_B1_16			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 16 )		/* DP-XMC-5775 Board 1, channel 16 */
#define SG_LVTTL_IO_B1_17			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 17 )		/* DP-XMC-5775 Board 1, channel 17 */
#define SG_LVTTL_IO_B1_18			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 18 )		/* DP-XMC-5775 Board 1, channel 18 */
#define SG_LVTTL_IO_B1_19			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 19 )		/* DP-XMC-5775 Board 1, channel 19 */
#define SG_LVTTL_IO_B1_20			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 20 )		/* DP-XMC-5775 Board 1, channel 20 */
#define SG_LVTTL_IO_B1_21			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 21 )		/* DP-XMC-5775 Board 1, channel 21 */
#define SG_LVTTL_IO_B1_22			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 22 )		/* DP-XMC-5775 Board 1, channel 22 */
#define SG_LVTTL_IO_B1_23			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 23 )		/* DP-XMC-5775 Board 1, channel 23 */
#define SG_LVTTL_IO_B1_24			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 24 )		/* DP-XMC-5775 Board 1, channel 24 */
#define SG_LVTTL_IO_B1_25			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 25 )		/* DP-XMC-5775 Board 1, channel 25 */
#define SG_LVTTL_IO_B1_26			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 26 )		/* DP-XMC-5775 Board 1, channel 26 */
#define SG_LVTTL_IO_B1_27			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 27 )		/* DP-XMC-5775 Board 1, channel 27 */
#define SG_LVTTL_IO_B1_28			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 28 )		/* DP-XMC-5775 Board 1, channel 28 */
#define SG_LVTTL_IO_B1_29			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 29 )		/* DP-XMC-5775 Board 1, channel 29 */
#define SG_LVTTL_IO_B1_30			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 30 )		/* DP-XMC-5775 Board 1, channel 30 */
#define SG_LVTTL_IO_B1_31			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 31 )		/* DP-XMC-5775 Board 1, channel 31 */
#define SG_LVTTL_IO_B1_32			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 32 )		/* DP-XMC-5775 Board 1, channel 32 */
#define SG_LVTTL_IO_B1_33			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 33 )		/* DP-XMC-5775 Board 1, channel 33 */
#define SG_LVTTL_IO_B1_34			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 34 )		/* DP-XMC-5775 Board 1, channel 34 */
#define SG_LVTTL_IO_B1_35			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 35 )		/* DP-XMC-5775 Board 1, channel 35 */
#define SG_LVTTL_IO_B1_36			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 36 ) 		/* DP-XMC-5775 Board 1, channel 36 */
#define SG_LVTTL_IO_B1_37			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 37 )		/* DP-XMC-5775 Board 1, channel 37 */
#define SG_LVTTL_IO_B1_38			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 38 )		/* DP-XMC-5775 Board 1, channel 38 */
#define SG_LVTTL_IO_B1_39			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 39 )		/* DP-XMC-5775 Board 1, channel 39 */
#define SG_LVTTL_IO_B1_40			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 40 )		/* DP-XMC-5775 Board 1, channel 40 */
#define SG_LVTTL_IO_B1_41			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 41 )		/* DP-XMC-5775 Board 1, channel 41 */
#define SG_LVTTL_IO_B1_42			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 42 )		/* DP-XMC-5775 Board 1, channel 42 */
#define SG_LVTTL_IO_B1_43			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 43 )		/* DP-XMC-5775 Board 1, channel 43 */
#define SG_LVTTL_IO_B1_44			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 44 )		/* DP-XMC-5775 Board 1, channel 44 */
#define SG_LVTTL_IO_B1_45			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 45 )		/* DP-XMC-5775 Board 1, channel 45 */
#define SG_LVTTL_IO_B1_46			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 46 )		/* DP-XMC-5775 Board 1, channel 46 */
#define SG_LVTTL_IO_B1_47			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 47 )		/* DP-XMC-5775 Board 1, channel 47 */
#define SG_LVTTL_IO_B1_48			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 48 )		/* DP-XMC-5775 Board 1, channel 48 */
#define SG_LVTTL_IO_B1_49			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 49 )		/* DP-XMC-5775 Board 1, channel 49 */
#define SG_LVTTL_IO_B1_50			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 50 )		/* DP-XMC-5775 Board 1, channel 50 */
#define SG_LVTTL_IO_B1_51			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 51 )		/* DP-XMC-5775 Board 1, channel 51 */
#define SG_LVTTL_IO_B1_52			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 52 )		/* DP-XMC-5775 Board 1, channel 52 */
#define SG_LVTTL_IO_B1_53			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 53 )		/* DP-XMC-5775 Board 1, channel 53 */
#define SG_LVTTL_IO_B1_54			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 54 )		/* DP-XMC-5775 Board 1, channel 54 */
#define SG_LVTTL_IO_B1_55			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 55 )		/* DP-XMC-5775 Board 1, channel 55 */
#define SG_LVTTL_IO_B1_56			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 56 )		/* DP-XMC-5775 Board 1, channel 56 */
#define SG_LVTTL_IO_B1_57			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 57 )		/* DP-XMC-5775 Board 1, channel 57 */
#define SG_LVTTL_IO_B1_58			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 58 )		/* DP-XMC-5775 Board 1, channel 58 */
#define SG_LVTTL_IO_B1_59			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 59 )		/* DP-XMC-5775 Board 1, channel 59 */
#define SG_LVTTL_IO_B1_60			( BRD_XMC5775_LVTTL + SID_BOARD_1 + 60 )		/* DP-XMC-5775 Board 1, channel 60 */


/* DP-XMC-5775 - LVTTL/LVDS/GPIO Input & Output*/
#define SG_LVTTL_GPIO_B1_CH21           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 21 )    /* DP-XMC-5775 Board 1, channel 21 */
#define SG_LVTTL_GPIO_B1_CH22           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 22 )    /* DP-XMC-5775 Board 1, channel 22 */
#define SG_LVTTL_GPIO_B1_CH23           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 23 )    /* DP-XMC-5775 Board 1, channel 23 */
#define SG_LVTTL_GPIO_B1_CH24           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 24 )    /* DP-XMC-5775 Board 1, channel 24 */
#define SG_LVTTL_GPIO_B1_CH25           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 25 )    /* DP-XMC-5775 Board 1, channel 25 */
#define SG_LVTTL_GPIO_B1_CH26           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 26 )    /* DP-XMC-5775 Board 1, channel 26 */
#define SG_LVTTL_GPIO_B1_CH27           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 27 )    /* DP-XMC-5775 Board 1, channel 27 */
#define SG_LVTTL_GPIO_B1_CH28           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 28 )    /* DP-XMC-5775 Board 1, channel 28 */
#define SG_LVTTL_GPIO_B1_CH29           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 29 )    /* DP-XMC-5775 Board 1, channel 29 */
#define SG_LVTTL_GPIO_B1_CH30           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 30 )    /* DP-XMC-5775 Board 1, channel 30 */
#define SG_LVTTL_GPIO_B1_CH31           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 31 )    /* DP-XMC-5775 Board 1, channel 31 */
#define SG_LVTTL_GPIO_B1_CH32           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 32 )    /* DP-XMC-5775 Board 1, channel 32 */
#define SG_LVTTL_GPIO_B1_CH33           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 33 )    /* DP-XMC-5775 Board 1, channel 33 */
#define SG_LVTTL_GPIO_B1_CH34           ( BRD_XMC5775_LVTTL + SID_BOARD_4 + 34 )    /* DP-XMC-5775 Board 1, channel 34 */

#define SG_LVDS_GPIO_B1_CH1              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 1  )   /* DP-XMC-5775 Board 1, channel 1  */
#define SG_LVDS_GPIO_B1_CH2              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 2  )   /* DP-XMC-5775 Board 1, channel 2  */
#define SG_LVDS_GPIO_B1_CH3              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 3  )   /* DP-XMC-5775 Board 1, channel 3  */
#define SG_LVDS_GPIO_B1_CH4              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 4  )   /* DP-XMC-5775 Board 1, channel 4  */
#define SG_LVDS_GPIO_B1_CH5              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 5  )   /* DP-XMC-5775 Board 1, channel 5  */
#define SG_LVDS_GPIO_B1_CH6              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 6  )   /* DP-XMC-5775 Board 1, channel 6  */
#define SG_LVDS_GPIO_B1_CH7              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 7  )   /* DP-XMC-5775 Board 1, channel 7  */
#define SG_LVDS_GPIO_B1_CH8              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 8  )   /* DP-XMC-5775 Board 1, channel 8  */
#define SG_LVDS_GPIO_B1_CH9              ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 9  )   /* DP-XMC-5775 Board 1, channel 9  */
#define SG_LVDS_GPIO_B1_CH10             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 10 )   /* DP-XMC-5775 Board 1, channel 10 */
#define SG_LVDS_GPIO_B1_CH11             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 11 )   /* DP-XMC-5775 Board 1, channel 11 */
#define SG_LVDS_GPIO_B1_CH12             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 12 )   /* DP-XMC-5775 Board 1, channel 12 */
#define SG_LVDS_GPIO_B1_CH13             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 13 )   /* DP-XMC-5775 Board 1, channel 13 */
#define SG_LVDS_GPIO_B1_CH14             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 14 )   /* DP-XMC-5775 Board 1, channel 14 */
#define SG_LVDS_GPIO_B1_CH15             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 15 )   /* DP-XMC-5775 Board 1, channel 15 */
#define SG_LVDS_GPIO_B1_CH16             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 16 )   /* DP-XMC-5775 Board 1, channel 16 */
#define SG_LVDS_GPIO_B1_CH17             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 17 )   /* DP-XMC-5775 Board 1, channel 17 */
#define SG_LVDS_GPIO_B1_CH18             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 18 )   /* DP-XMC-5775 Board 1, channel 18 */
#define SG_LVDS_GPIO_B1_CH19             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 19 )   /* DP-XMC-5775 Board 1, channel 19 */
#define SG_LVDS_GPIO_B1_CH20             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 20 )   /* DP-XMC-5775 Board 1, channel 20 */
#define SG_LVDS_GPIO_B1_CH35             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 35 )   /* DP-XMC-5775 Board 1, channel 35 */
#define SG_LVDS_GPIO_B1_CH36             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 36 )   /* DP-XMC-5775 Board 1, channel 36 */
#define SG_LVDS_GPIO_B1_CH37             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 37 )   /* DP-XMC-5775 Board 1, channel 37 */
#define SG_LVDS_GPIO_B1_CH38             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 38 )   /* DP-XMC-5775 Board 1, channel 38 */
#define SG_LVDS_GPIO_B1_CH39             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 39 )   /* DP-XMC-5775 Board 1, channel 39 */
#define SG_LVDS_GPIO_B1_CH40             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 40 )   /* DP-XMC-5775 Board 1, channel 40 */
#define SG_LVDS_GPIO_B1_CH41             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 41 )   /* DP-XMC-5775 Board 1, channel 41 */
#define SG_LVDS_GPIO_B1_CH42             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 42 )   /* DP-XMC-5775 Board 1, channel 42 */
#define SG_LVDS_GPIO_B1_CH43             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 43 )   /* DP-XMC-5775 Board 1, channel 43 */
#define SG_LVDS_GPIO_B1_CH44             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 44 )   /* DP-XMC-5775 Board 1, channel 44 */
#define SG_LVDS_GPIO_B1_CH45             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 45 )   /* DP-XMC-5775 Board 1, channel 45 */
#define SG_LVDS_GPIO_B1_CH46             ( BRD_XMC5775_LVDS  + SID_BOARD_4 + 46 )   /* DP-XMC-5775 Board 1, channel 46 */


#endif // DP_SBAND_RADAR_SIGNALS_H
