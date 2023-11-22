// Preprocessing -> --define
#ifdef CLA_C
// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are.
CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C

MEMORY
{
PAGE 0 :  /* Program Memory */
          /* Memory (RAM/FLASH) blocks can be moved to PAGE1 for data allocation */

   RAMM0           	: origin = 0x000122,   length = 0x0002DE /* reserved for datalog */
   RAMD0_1           	: origin = 0x00B000,   length = 0x001000 /* TI.ramfunc */

   RAMLS4_5         : origin = 0x00A000,   length = 0x001000 /*CLA_PRG*/
   
   RAMGS14          : origin = 0x01A000,   length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS15          : origin = 0x01B000,   length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

   RESET           	: origin = 0x3FFFC0,   length = 0x000002

   /* Flash sectors */
   BEGIN           	: origin = 0x080000,   length = 0x000002	/* BEGIN is used for the "boot to SARAM" bootloader mode  or codestart section  */
   BOOTVEC 	   		: origin = 0x080002,   length = 0x000050
   FLASHA           : origin = 0x080052,   length = 0x001FAE/*length = 0x001FFE	/* on-chip Flash ---8K*/
   FLASHB           : origin = 0x082000,   length = 0x002000	/* on-chip Flash ---8K*/
   FLASHC           : origin = 0x084000,   length = 0x002000	/* on-chip Flash ---8K*/
   FLASHD           : origin = 0x086000,   length = 0x002000	/* on-chip Flash ---8K*/
   FLASHE           : origin = 0x088000,   length = 0x008000	/* on-chip Flash ---32K*/
   FLASHF           : origin = 0x090000,   length = 0x008000	/* on-chip Flash ---32K*/
   FLASHG           : origin = 0x098000,   length = 0x008000	/* on-chip Flash ---32K*/
   FLASHH           : origin = 0x0A0000,   length = 0x008000	/* on-chip Flash ---32K*/
   FLASHI           : origin = 0x0A8000,   length = 0x008000	/* on-chip Flash ---32K*/
   FLASHJ           : origin = 0x0B0000,   length = 0x008000	/* on-chip Flash ---32K*/
   FLASHK           : origin = 0x0B8000,   length = 0x002000	/* on-chip Flash ---8K*/
   FLASHL           : origin = 0x0BA000,   length = 0x002000	/* on-chip Flash ---8K*/
   FLASHM           : origin = 0x0BC000,   length = 0x002000	/* on-chip Flash ---8K*/
/*   FLASHN           : origin = 0x0BE000,   length = 0x002000	/* on-chip Flash ---8K*/
   LIB_FLASH   		: origin = 0x0BE000,   length = 0x001000
   FLASH_BOOT  		: origin = 0x0BF000,   length = 0x001000     /* on-chip FLASH */

PAGE 1 : /* Data Memory */
         /* Memory (RAM/FLASH) blocks can be moved to PAGE0 for program allocation */

   BOOT_RSVD        : origin = 0x000002,   length = 0x000120     /* Part of M0, BOOT rom will use this for stack */
   RAMM1            : origin = 0x000400,   length = 0x000300     /* stack: on-chip RAM block M1 */

   IsrVariable 		: origin = 0x00700, 	length = 0x0000E0

	/*CLA1 RAM: LS0_5 can be used as CLA_PRG or CLA_RAM*/
   RAMLS0          	: origin = 0x008000,   length = 0x000800 /*2K*/
   RAMLS1          	: origin = 0x008800,   length = 0x000700
   RAMLS1_RSVD      : origin = 0x008F00,   length = 0x000100


	/*CPU RAM*/
   RAMLS2_3      		: origin = 0x009000,   length = 0x001000


	/*CPU1 RAM*/
   RAMGS0           : origin = 0x00C000,   length = 0x001000 /* Filter_RegsFile */
   RAMGS1           : origin = 0x00D000,   length = 0x001000
   RAMGS2           : origin = 0x00E000,   length = 0x001000
   RAMGS3           : origin = 0x00F000,   length = 0x001000
   RAMGS4           : origin = 0x010000,   length = 0x001000
   RAMGS5           : origin = 0x011000,   length = 0x001000
	/*CPU2 RAM*/
   RAMGS6           : origin = 0x012000,   length = 0x001000
   RAMGS7           : origin = 0x013000,   length = 0x001000
   RAMGS8           : origin = 0x014000,   length = 0x001000
   RAMGS9           : origin = 0x015000,   length = 0x001000
   RAMGS10          : origin = 0x016000,   length = 0x001000
   RAMGS11          : origin = 0x017000,   length = 0x001000

	/*Unused*/
   RAMGS12          : origin = 0x018000,   length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */
   RAMGS13          : origin = 0x019000,   length = 0x001000     /* Only Available on F28379D, F28377D, F28375D devices. Remove line on other devices. */

   CANA_MSG_RAM     : origin = 0x049000,   length = 0x000800
   CANB_MSG_RAM     : origin = 0x04B000,   length = 0x000800

   CLA1_MSGRAMLOW  	: origin = 0x001480,   length = 0x000080 /* CLA to CPU Message */
   CLA1_MSGRAMHIGH  : origin = 0x001500,   length = 0x000080 /* CPU to CLA Message */

}


SECTIONS
{
/*****************Allocate program areas:********************************************/
   /* .cinit -- Tables for explicitly initialized global and static variables. */
   .cinit           : > FLASHB      PAGE = 0, ALIGN(4)
   .pinit           : > FLASHB,     PAGE = 0, ALIGN(4)
   .text            : > FLASHA | FLASHB | FLASHC | FLASHD  | FLASHE      PAGE = 0, ALIGN(4)
   codestart        : > BEGIN       PAGE = 0, ALIGN(4)

/*****************Allocate uninitalized data sections: (RAM) ******************************/
   .stack           : > RAMM1        PAGE = 1
   .ebss            : > RAMLS1_RSVD|RAMLS2_3       PAGE = 1
/****************Initalized sections go in Flash£º**************************************/
   .econst          : > FLASHB      PAGE = 0, ALIGN(4)
   .switch          : > FLASHB      PAGE = 0, ALIGN(4)

   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */

   SFRA_F_Data		: > RAMLS2_3, ALIGN = 64, PAGE = 1
   SFRA_Data		: > RAMLS2_3, ALIGN = 64, PAGE = 1

   Filter_RegsFile  : > RAMGS0,	   PAGE = 1
   
    /* CLA specific sections */
   Cla1Prog         : LOAD = FLASHD,
                      RUN = RAMLS4_5,
                      LOAD_START(_Cla1funcsLoadStart),
                      LOAD_END(_Cla1funcsLoadEnd),
                      RUN_START(_Cla1funcsRunStart),
                      LOAD_SIZE(_Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(4)

   CLADataLS0		: > RAMLS0, PAGE=1 /* CLA read, CPU Write */
   CLADataLS1		: > RAMLS1, PAGE=1 /* CLA read, CPU Write*/

   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW,   PAGE = 1, ALIGN(4)
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH,  PAGE = 1, ALIGN(4)

   CpuToCla1MsgRAM  : > RAMGS5,  PAGE = 1

      GROUP
   {
       .TI.ramfunc
       ramfuncs
 	   IsrRamfuncs
   }LOAD = FLASHD,
	RUN = RAMD0_1,
	LOAD_START(_RamfuncsLoadStart),
    LOAD_SIZE(_RamfuncsLoadSize),
    LOAD_END(_RamfuncsLoadEnd),
    RUN_START(_RamfuncsRunStart),
    RUN_SIZE(_RamfuncsRunSize),
    RUN_END(_RamfuncsRunEnd),
	PAGE = 0, ALIGN(4)

   /* The following section definition are for SDFM examples */
   Filter1_RegsFile : > RAMGS1,	PAGE = 1, fill=0x1111
   Filter2_RegsFile : > RAMGS2,	PAGE = 1, fill=0x2222
   Filter3_RegsFile : > RAMGS3,	PAGE = 1, fill=0x3333
   Filter4_RegsFile : > RAMGS4,	PAGE = 1, fill=0x4444

#ifdef CLA_C
   .scratchpad      : > RAMLS1,       PAGE = 1
   .bss_cla		    : > RAMLS1,       PAGE = 1
   .const_cla	    :  LOAD = FLASHB,
                       RUN = RAMLS1,
                       RUN_START(_Cla1ConstRunStart),
                       LOAD_START(_Cla1ConstLoadStart),
                       LOAD_SIZE(_Cla1ConstLoadSize),
                       PAGE = 1
#endif //CLA_C

   /* Allocate IQ math areas: */
   IQmath			: > FLASHB, PAGE = 0, ALIGN(4)            /* Math Code */
   IQmathTables		: > FLASHC, PAGE = 0, ALIGN(4)

/******************************************************************************************************/
   IsrVariableFile	 : > IsrVariable  	PAGE = 1
   .bootvec       	 : > BOOTVEC  		PAGE = 0
	FlashBoot           : > FLASH_BOOT      PAGE = 0
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
