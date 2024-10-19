/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 1998 Bero
 *  Copyright (C) 2002 Xodnizel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _FCEU_INES_H
#define _FCEU_INES_H

typedef struct {
	char ID[4];					/* NES^Z */
	uint8 ROM_size;				/* prg page count (16K) */
	uint8 VROM_size;			/* chr page count (8K) */
	uint8 ROM_type;				/* byte6 */
	uint8 ROM_type2;			/* byte7 */
	uint8 ROM_type3;			/* byte8 */
	uint8 upper_PRG_CHR_size;	/* byte9 */
	uint8 PRGRAM_size;			/* byte10 */
	uint8 CHRRAM_size;			/* byte11 */
	uint8 Region;				/* byte12 */
	uint8 VS_hardware;			/* byte13 */
	uint8 MiscRoms;				/* byte14 */
	uint8 ExpDevice;			/* byte15 */
} iNES_HEADER;

<<<<<<< HEAD
extern uint8 *ROM;
extern uint8 *VROM;
extern uint32 ROM_size;		/* prg size in 16K chunks */
extern uint32 VROM_size;	/* chr size in 8K chunks */
extern iNES_HEADER head;

void NSFVRC6_Init(void);
void NSFMMC5_Init(void);
void NSFAY_Init(void);
void NSFN106_Init(void);
void NSFVRC7_Init(void);

void Mapper1_Init(CartInfo *);
void Mapper4_Init(CartInfo *);
void Mapper5_Init(CartInfo *);
void Mapper6_Init(CartInfo *);
void Mapper8_Init(CartInfo *);
void Mapper9_Init(CartInfo *);
void Mapper10_Init(CartInfo *);
void Mapper11_Init(CartInfo *);
void Mapper12_Init(CartInfo *);
void Mapper15_Init(CartInfo *);
void Mapper16_Init(CartInfo *);
void Mapper17_Init(CartInfo *);
void Mapper18_Init(CartInfo *);
void Mapper19_Init(CartInfo *);
void Mapper21_Init(CartInfo *);
void Mapper22_Init(CartInfo *);
void Mapper23_Init(CartInfo *);
void Mapper24_Init(CartInfo *);
void Mapper25_Init(CartInfo *);
void Mapper26_Init(CartInfo *);
void Mapper28_Init(CartInfo *);
void Mapper29_Init(CartInfo *);
void Mapper31_Init(CartInfo *);
void Mapper32_Init(CartInfo *);
void Mapper33_Init(CartInfo *);
void Mapper34_Init(CartInfo *);
void Mapper35_Init(CartInfo *);
void Mapper36_Init(CartInfo *);
void Mapper37_Init(CartInfo *);
void Mapper38_Init(CartInfo *);
void Mapper40_Init(CartInfo *);
void Mapper41_Init(CartInfo *);
void Mapper42_Init(CartInfo *);
void Mapper43_Init(CartInfo *);
void Mapper44_Init(CartInfo *);
void Mapper45_Init(CartInfo *);
void Mapper46_Init(CartInfo *);
void Mapper47_Init(CartInfo *);
void Mapper48_Init(CartInfo *);
void Mapper49_Init(CartInfo *);
void Mapper50_Init(CartInfo *);
void Mapper51_Init(CartInfo *);
void Mapper52_Init(CartInfo *);
void Mapper57_Init(CartInfo *);
void Mapper58_Init(CartInfo *);
void Mapper59_Init(CartInfo *);
void Mapper60_Init(CartInfo *);
void Mapper61_Init(CartInfo *);
void Mapper62_Init(CartInfo *);
void Mapper63_Init(CartInfo *);
void Mapper64_Init(CartInfo *);
void Mapper65_Init(CartInfo *);
void Mapper67_Init(CartInfo *);
void Mapper68_Init(CartInfo *);
void Mapper69_Init(CartInfo *);
void Mapper70_Init(CartInfo *);
void Mapper71_Init(CartInfo *);
void Mapper72_Init(CartInfo *);
void Mapper73_Init(CartInfo *);
void Mapper74_Init(CartInfo *);
void Mapper75_Init(CartInfo *);
void Mapper76_Init(CartInfo *);
void Mapper77_Init(CartInfo *);
void Mapper78_Init(CartInfo *);
void Mapper79_Init(CartInfo *);
void Mapper80_Init(CartInfo *);
void Mapper81_Init(CartInfo *);
void Mapper82_Init(CartInfo *);
void Mapper83_Init(CartInfo *);
void Mapper85_Init(CartInfo *);
void Mapper86_Init(CartInfo *);
void Mapper87_Init(CartInfo *);
void Mapper88_Init(CartInfo *);
void Mapper89_Init(CartInfo *);
void Mapper90_Init(CartInfo *);
void Mapper91_Init(CartInfo *);
void Mapper92_Init(CartInfo *);
void Mapper93_Init(CartInfo *);
void Mapper94_Init(CartInfo *);
void Mapper95_Init(CartInfo *);
void Mapper96_Init(CartInfo *);
void Mapper97_Init(CartInfo *);
void Mapper99_Init(CartInfo *);
=======
void Mapper000_Init(CartInfo *);
void Mapper001_Init(CartInfo *);
void Mapper002_Init(CartInfo *);
void Mapper003_Init(CartInfo *);
void Mapper004_Init(CartInfo *);
void Mapper005_Init(CartInfo *);
void Mapper006_Init(CartInfo *);
void Mapper007_Init(CartInfo *);
void Mapper008_Init(CartInfo *);
void Mapper009_Init(CartInfo *);
void Mapper010_Init(CartInfo *);
void Mapper011_Init(CartInfo *);
void Mapper012_Init(CartInfo *);
void Mapper013_Init(CartInfo *);
void Mapper014_Init(CartInfo *);
void Mapper015_Init(CartInfo *);
void Mapper016_Init(CartInfo *);
void Mapper017_Init(CartInfo *);
void Mapper018_Init(CartInfo *);
void Mapper019_Init(CartInfo *);
void Mapper021_Init(CartInfo *);
void Mapper022_Init(CartInfo *);
void Mapper023_Init(CartInfo *);
void Mapper024_Init(CartInfo *);
void Mapper025_Init(CartInfo *);
void Mapper026_Init(CartInfo *);
void Mapper027_Init(CartInfo *);
void Mapper028_Init(CartInfo *);
void Mapper029_Init(CartInfo *);
void Mapper030_Init(CartInfo *);
void Mapper031_Init(CartInfo *);
void Mapper032_Init(CartInfo *);
void Mapper033_Init(CartInfo *);
void Mapper034_Init(CartInfo *);
void Mapper035_Init(CartInfo *);
void Mapper036_Init(CartInfo *);
void Mapper037_Init(CartInfo *);
void Mapper038_Init(CartInfo *);
void Mapper040_Init(CartInfo *);
void Mapper041_Init(CartInfo *);
void Mapper042_Init(CartInfo *);
void Mapper043_Init(CartInfo *);
void Mapper044_Init(CartInfo *);
void Mapper045_Init(CartInfo *);
void Mapper046_Init(CartInfo *);
void Mapper047_Init(CartInfo *);
void Mapper048_Init(CartInfo *);
void Mapper049_Init(CartInfo *);
void Mapper050_Init(CartInfo *);
void Mapper051_Init(CartInfo *);
void Mapper052_Init(CartInfo *);
void Mapper053_Init(CartInfo *);
void Mapper055_Init(CartInfo *);
void Mapper056_Init(CartInfo *);
void Mapper057_Init(CartInfo *);
void Mapper058_Init(CartInfo *);
void Mapper059_Init(CartInfo *);
void Mapper060_Init(CartInfo *);
void Mapper061_Init(CartInfo *);
void Mapper062_Init(CartInfo *);
void Mapper063_Init(CartInfo *);
void Mapper064_Init(CartInfo *);
void Mapper065_Init(CartInfo *);
void Mapper066_Init(CartInfo *);
void Mapper067_Init(CartInfo *);
void Mapper068_Init(CartInfo *);
void Mapper069_Init(CartInfo *);
void Mapper070_Init(CartInfo *);
void Mapper071_Init(CartInfo *);
void Mapper072_Init(CartInfo *);
void Mapper073_Init(CartInfo *);
void Mapper074_Init(CartInfo *);
void Mapper075_Init(CartInfo *);
void Mapper076_Init(CartInfo *);
void Mapper077_Init(CartInfo *);
void Mapper078_Init(CartInfo *);
void Mapper079_Init(CartInfo *);
void Mapper080_Init(CartInfo *);
void Mapper081_Init(CartInfo *);
void Mapper082_Init(CartInfo *);
void Mapper083_Init(CartInfo *);
void Mapper085_Init(CartInfo *);
void Mapper086_Init(CartInfo *);
void Mapper087_Init(CartInfo *);
void Mapper088_Init(CartInfo *);
void Mapper089_Init(CartInfo *);
void Mapper090_Init(CartInfo *);
void Mapper091_Init(CartInfo *);
/* void Mapper092_Init(CartInfo *); */
void Mapper093_Init(CartInfo *);
void Mapper094_Init(CartInfo *);
void Mapper095_Init(CartInfo *);
void Mapper096_Init(CartInfo *);
void Mapper097_Init(CartInfo *);
void Mapper099_Init(CartInfo *);
>>>>>>> c91f457 (Update libretro.c)
void Mapper101_Init(CartInfo *);
void Mapper103_Init(CartInfo *);
void Mapper104_Init(CartInfo *);
void Mapper105_Init(CartInfo *);
void Mapper106_Init(CartInfo *);
void Mapper107_Init(CartInfo *);
void Mapper108_Init(CartInfo *);
void Mapper111_Init(CartInfo *);
void Mapper112_Init(CartInfo *);
void Mapper113_Init(CartInfo *);
void Mapper114_Init(CartInfo *);
void Mapper115_Init(CartInfo *);
void Mapper116_Init(CartInfo *);
void Mapper117_Init(CartInfo *);
void Mapper118_Init(CartInfo *);
void Mapper119_Init(CartInfo *);
void Mapper120_Init(CartInfo *);
void Mapper121_Init(CartInfo *);
void Mapper123_Init(CartInfo *);
void Mapper124_Init(CartInfo *);
void Mapper125_Init(CartInfo *);
void Mapper126_Init(CartInfo *);
<<<<<<< HEAD
=======
void Mapper127_Init(CartInfo *);
>>>>>>> 00e3626 (Update libretro.c)
void Mapper128_Init(CartInfo *);
void Mapper132_Init(CartInfo *);
void Mapper133_Init(CartInfo *);
void Mapper134_Init(CartInfo *);
void Mapper136_Init(CartInfo *);
void Mapper137_Init(CartInfo *);
void Mapper138_Init(CartInfo *);
void Mapper139_Init(CartInfo *);
void Mapper140_Init(CartInfo *);
void Mapper141_Init(CartInfo *);
void Mapper142_Init(CartInfo *);
void Mapper143_Init(CartInfo *);
void Mapper144_Init(CartInfo *);
void Mapper145_Init(CartInfo *);
void Mapper147_Init(CartInfo *);
void Mapper148_Init(CartInfo *);
void Mapper149_Init(CartInfo *);
void Mapper150_Init(CartInfo *);
void Mapper151_Init(CartInfo *);
void Mapper152_Init(CartInfo *);
void Mapper153_Init(CartInfo *);
void Mapper154_Init(CartInfo *);
void Mapper155_Init(CartInfo *);
void Mapper156_Init(CartInfo *);
void Mapper157_Init(CartInfo *);
void Mapper159_Init(CartInfo *);
void Mapper162_Init(CartInfo *);
void Mapper163_Init(CartInfo *);
void Mapper164_Init(CartInfo *);
void Mapper165_Init(CartInfo *);
void Mapper166_Init(CartInfo *);
void Mapper167_Init(CartInfo *);
void Mapper168_Init(CartInfo *);
void Mapper170_Init(CartInfo *);
void Mapper171_Init(CartInfo *); /* Kaiser 7058 */
void Mapper172_Init(CartInfo *);
void Mapper173_Init(CartInfo *);
void Mapper174_Init(CartInfo *);
void Mapper175_Init(CartInfo *);
void Mapper176_Init(CartInfo *);
void Mapper177_Init(CartInfo *);
void Mapper178_Init(CartInfo *);
void Mapper180_Init(CartInfo *);
void Mapper183_Init(CartInfo *);
void Mapper184_Init(CartInfo *);
void Mapper185_Init(CartInfo *);
void Mapper186_Init(CartInfo *);
void Mapper187_Init(CartInfo *);
void Mapper188_Init(CartInfo *);
void Mapper189_Init(CartInfo *);
void Mapper190_Init(CartInfo *);
void Mapper191_Init(CartInfo *);
void Mapper192_Init(CartInfo *);
void Mapper193_Init(CartInfo *);
void Mapper194_Init(CartInfo *);
void Mapper195_Init(CartInfo *);
void Mapper196_Init(CartInfo *);
void Mapper197_Init(CartInfo *);
void Mapper198_Init(CartInfo *);
void Mapper199_Init(CartInfo *);
void Mapper200_Init(CartInfo *);
void Mapper201_Init(CartInfo *);
void Mapper202_Init(CartInfo *);
void Mapper203_Init(CartInfo *);
void Mapper204_Init(CartInfo *);
void Mapper205_Init(CartInfo *);
void Mapper206_Init(CartInfo *);
void Mapper207_Init(CartInfo *);
void Mapper208_Init(CartInfo *);
void Mapper209_Init(CartInfo *);
void Mapper210_Init(CartInfo *);
void Mapper211_Init(CartInfo *);
void Mapper212_Init(CartInfo *);
void Mapper213_Init(CartInfo *);
void Mapper214_Init(CartInfo *);
void Mapper215_Init(CartInfo *);
void Mapper216_Init(CartInfo *);
void Mapper217_Init(CartInfo *);
void Mapper218_Init(CartInfo *);
void Mapper219_Init(CartInfo *);
void Mapper220_Init(CartInfo *);
void Mapper221_Init(CartInfo *);
void Mapper222_Init(CartInfo *);
void Mapper224_Init(CartInfo *);
void Mapper225_Init(CartInfo *);
void Mapper226_Init(CartInfo *);
void Mapper227_Init(CartInfo *);
void Mapper228_Init(CartInfo *);
void Mapper229_Init(CartInfo *);
void Mapper230_Init(CartInfo *);
void Mapper231_Init(CartInfo *);
void Mapper232_Init(CartInfo *);
void Mapper233_Init(CartInfo *);
void Mapper234_Init(CartInfo *);
void Mapper235_Init(CartInfo *);
void Mapper236_Init(CartInfo *);
void Mapper237_Init(CartInfo *);
void Mapper238_Init(CartInfo *);
void Mapper240_Init(CartInfo *);
void Mapper241_Init(CartInfo *);
void Mapper242_Init(CartInfo *);
void Mapper244_Init(CartInfo *);
void Mapper245_Init(CartInfo *);
void Mapper246_Init(CartInfo *);
void Mapper249_Init(CartInfo *);
void Mapper250_Init(CartInfo *);
void Mapper252_Init(CartInfo *);
void Mapper253_Init(CartInfo *);
void Mapper254_Init(CartInfo *);
void Mapper255_Init(CartInfo *);
<<<<<<< HEAD

void GN45_Init(CartInfo *info); /* m361, m366 */
void Mapper272_Init(CartInfo *);
void Mapper277_Init(CartInfo *);
void Mapper281_Init(CartInfo *);
void Mapper282_Init(CartInfo *);
void Mapper283_Init(CartInfo *);
void Mapper291_Init(CartInfo *);
void Mapper295_Init(CartInfo *);

void J2282_Init(CartInfo *);

void Mapper265_Init(CartInfo *);
=======
void Mapper256_Init(CartInfo *);
void Mapper257_Init(CartInfo *);
void Mapper259_Init(CartInfo *);
void Mapper260_Init(CartInfo *);
void Mapper261_Init(CartInfo *);
void Mapper262_Init(CartInfo *);
void Mapper263_Init(CartInfo *);
void Mapper264_Init(CartInfo *);
void Mapper265_Init(CartInfo *);
void Mapper266_Init(CartInfo *);
>>>>>>> 1f68903 (Update libretro.c)
void Mapper267_Init(CartInfo *);
void Mapper268_Init(CartInfo *);
void Mapper269_Init(CartInfo *);
void Mapper271_Init(CartInfo *);
void Mapper272_Init(CartInfo *);
void Mapper274_Init(CartInfo *);
void Mapper277_Init(CartInfo *);
void Mapper281_Init(CartInfo *);
void Mapper282_Init(CartInfo *);
void Mapper283_Init(CartInfo *);
void Mapper285_Init(CartInfo *);
void Mapper286_Init(CartInfo *);
void Mapper287_Init(CartInfo *);
void Mapper288_Init(CartInfo *);
void Mapper289_Init(CartInfo *);
void Mapper290_Init(CartInfo *);
void Mapper291_Init(CartInfo *);
void Mapper292_Init(CartInfo *);
void Mapper293_Init(CartInfo *);
void Mapper294_Init(CartInfo *);
void Mapper295_Init(CartInfo *);
void Mapper297_Init(CartInfo *);
void Mapper298_Init(CartInfo *);
void Mapper299_Init(CartInfo *);
void Mapper300_Init(CartInfo *);
void Mapper301_Init(CartInfo *);
void Mapper302_Init(CartInfo *);
void Mapper303_Init(CartInfo *);
void Mapper304_Init(CartInfo *);
void Mapper305_Init(CartInfo *);
void Mapper306_Init(CartInfo *);
void Mapper307_Init(CartInfo *);
void Mapper308_Init(CartInfo *);
void Mapper309_Init(CartInfo *);
void Mapper310_Init(CartInfo *);
void Mapper312_Init(CartInfo *);
void Mapper313_Init(CartInfo *);
void Mapper314_Init(CartInfo *);
void Mapper315_Init(CartInfo *);
void Mapper319_Init(CartInfo *);
void Mapper320_Init(CartInfo *);
void Mapper322_Init(CartInfo *);
void Mapper323_Init(CartInfo *);
void Mapper324_Init(CartInfo *);
void Mapper325_Init(CartInfo *);
void Mapper326_Init(CartInfo *);
void Mapper327_Init(CartInfo *);
void Mapper328_Init(CartInfo *);
void Mapper329_Init(CartInfo *);
void Mapper330_Init(CartInfo *);
void Mapper331_Init(CartInfo *);
void Mapper332_Init(CartInfo *);
void Mapper333_Init(CartInfo *);
void Mapper334_Init(CartInfo *);
void Mapper335_Init(CartInfo *);
void Mapper336_Init(CartInfo *);
void Mapper337_Init(CartInfo *);
void Mapper338_Init(CartInfo *);
void Mapper339_Init(CartInfo *);
void Mapper340_Init(CartInfo *);
void Mapper341_Init(CartInfo *);
void Mapper343_Init(CartInfo *);
void Mapper344_Init(CartInfo *);
void Mapper345_Init(CartInfo *);
void Mapper346_Init(CartInfo *);
void Mapper347_Init(CartInfo *);
void Mapper348_Init(CartInfo *);
void Mapper349_Init(CartInfo *);
void Mapper350_Init(CartInfo *);
void Mapper351_Init(CartInfo *);
void Mapper352_Init(CartInfo *);
void Mapper353_Init(CartInfo *);
void Mapper354_Init(CartInfo *);
void Mapper355_Init(CartInfo *);
void Mapper356_Init(CartInfo *);
void Mapper357_Init(CartInfo *);
void Mapper358_Init(CartInfo *);
void Mapper359_Init(CartInfo *);
void Mapper360_Init(CartInfo *);
<<<<<<< HEAD
=======
void Mapper361_Init(CartInfo *);
>>>>>>> 00e3626 (Update libretro.c)
void Mapper362_Init(CartInfo *);
void Mapper364_Init(CartInfo *);
void Mapper366_Init(CartInfo *);
void Mapper368_Init(CartInfo *);
void Mapper369_Init(CartInfo *);
void Mapper370_Init(CartInfo *);
void Mapper372_Init(CartInfo *);
void Mapper374_Init(CartInfo *);
void Mapper375_Init(CartInfo *);
void Mapper376_Init(CartInfo *);
void Mapper377_Init(CartInfo *);
void Mapper380_Init(CartInfo *);
void Mapper381_Init(CartInfo *);
void Mapper382_Init(CartInfo *);
void Mapper383_Init(CartInfo *);
void Mapper385_Init(CartInfo *);
void Mapper386_Init(CartInfo *);
void Mapper387_Init(CartInfo *);
void Mapper388_Init(CartInfo *);
void Mapper389_Init(CartInfo *);
void Mapper390_Init(CartInfo *);
void Mapper391_Init(CartInfo *);
void Mapper393_Init(CartInfo *);
void Mapper394_Init(CartInfo *);
void Mapper395_Init(CartInfo *);
void Mapper396_Init(CartInfo *);
void Mapper397_Init(CartInfo *);
void Mapper398_Init(CartInfo *);
<<<<<<< HEAD
=======
void Mapper399_Init(CartInfo *);
void Mapper400_Init(CartInfo *);
>>>>>>> 00e3626 (Update libretro.c)
void Mapper401_Init(CartInfo *);
void Mapper402_Init(CartInfo *);
void Mapper403_Init(CartInfo *);
void Mapper404_Init(CartInfo *);
void Mapper406_Init(CartInfo *);
void Mapper409_Init(CartInfo *);
void Mapper410_Init(CartInfo *);
void Mapper411_Init(CartInfo *);
void Mapper412_Init(CartInfo *);
<<<<<<< HEAD
=======
void Mapper413_Init(CartInfo *);
>>>>>>> 975c80f (Update libretro.c)
void Mapper414_Init(CartInfo *);
void Mapper415_Init(CartInfo *);
void Mapper416_Init(CartInfo *);
void Mapper417_Init(CartInfo *);
<<<<<<< HEAD
=======
void Mapper418_Init(CartInfo *);
>>>>>>> 950f415 (Update libretro.c)
void Mapper420_Init(CartInfo *);
void Mapper421_Init(CartInfo *);
void Mapper422_Init(CartInfo *);
void Mapper428_Init(CartInfo *);
void Mapper429_Init(CartInfo *);
void Mapper430_Init(CartInfo *);
void Mapper431_Init(CartInfo *);
void Mapper432_Init(CartInfo *);
void Mapper433_Init(CartInfo *);
void Mapper434_Init(CartInfo *);
void Mapper435_Init(CartInfo *);
void Mapper436_Init(CartInfo *);
void Mapper437_Init(CartInfo *);
void Mapper438_Init(CartInfo *);
void Mapper439_Init(CartInfo *);
void Mapper441_Init(CartInfo *);
void Mapper443_Init(CartInfo *);
void Mapper444_Init(CartInfo *);
<<<<<<< HEAD
=======
void Mapper445_Init(CartInfo *);
void Mapper446_Init(CartInfo *);
void Mapper447_Init(CartInfo *);
>>>>>>> c8a10a4 (Update libretro.c)
void Mapper448_Init(CartInfo *);
void Mapper449_Init(CartInfo *);
void Mapper450_Init(CartInfo *);
void Mapper451_Init(CartInfo *);
void Mapper452_Init(CartInfo *);
void Mapper453_Init(CartInfo *);
<<<<<<< HEAD
void Mapper454_Init(CartInfo *);
=======
<<<<<<< HEAD
=======
void Mapper454_Init(CartInfo *);
>>>>>>> 950f415 (Update libretro.c)
>>>>>>> 00e3626 (Update libretro.c)
void Mapper455_Init(CartInfo *);
void Mapper456_Init(CartInfo *);
void Mapper457_Init(CartInfo *);
void Mapper458_Init(CartInfo *);
void Mapper459_Init(CartInfo *);
void Mapper460_Init(CartInfo *);
void Mapper461_Init(CartInfo *);
void Mapper463_Init(CartInfo *);
void Mapper464_Init(CartInfo *);
void Mapper465_Init(CartInfo *);
void Mapper466_Init(CartInfo *);
void Mapper467_Init(CartInfo *);
<<<<<<< HEAD
void Mapper468_Init(CartInfo *);
void INX_007T_Init(CartInfo* info); /* Mapper 470 */
void Mapper500_Init(CartInfo *);
void Mapper501_Init(CartInfo *);
void Mapper502_Init(CartInfo *);
=======
void Mapper471_Init(CartInfo *);
void Mapper500_Init(CartInfo *);
void Mapper501_Init(CartInfo *);
void Mapper502_Init(CartInfo *);
void Mapper512_Init(CartInfo *);
void Mapper513_Init(CartInfo *);
void Mapper514_Init(CartInfo *);
>>>>>>> 209715c (Update libretro.c)
void Mapper516_Init(CartInfo *);
void Mapper517_Init(CartInfo *);
void Mapper518_Init(CartInfo *);
void Mapper519_Init(CartInfo *);
void Mapper520_Init(CartInfo *);
void Mapper521_Init(CartInfo *);
void Mapper522_Init(CartInfo *);
void Mapper523_Init(CartInfo *);
<<<<<<< HEAD
void Mapper528_Init(CartInfo *);
=======
void Mapper524_Init(CartInfo *);
void Mapper525_Init(CartInfo *);
void Mapper526_Init(CartInfo *);
void Mapper527_Init(CartInfo *);
void Mapper528_Init(CartInfo *);
void Mapper529_Init(CartInfo *);
void Mapper530_Init(CartInfo *);
>>>>>>> 209715c (Update libretro.c)
void Mapper533_Init(CartInfo *);
void Mapper534_Init(CartInfo *);
void Mapper535_Init(CartInfo *);
void Mapper538_Init(CartInfo *);
void Mapper539_Init(CartInfo *);
void Mapper540_Init(CartInfo *);
void Mapper541_Init(CartInfo *);
void Mapper543_Init(CartInfo *);
void Mapper544_Init(CartInfo *);
void Mapper547_Init(CartInfo *);
void Mapper548_Init(CartInfo *);
void Mapper549_Init(CartInfo *);
void Mapper550_Init(CartInfo *);
void Mapper552_Init(CartInfo *);
void Mapper553_Init(CartInfo *);
void Mapper554_Init(CartInfo *);
<<<<<<< HEAD
void Mapper555_Init(CartInfo *);
=======
<<<<<<< HEAD
>>>>>>> 7b77b01 (Update libretro.c)
void Mapper556_Init(CartInfo *);
=======
void Mapper555_Init(CartInfo *);
void Mapper556_Init(CartInfo *);
void Mapper557_Init(CartInfo *);
>>>>>>> c8a10a4 (Update libretro.c)
void Mapper558_Init(CartInfo *);
void Mapper559_Init(CartInfo *);

#endif
