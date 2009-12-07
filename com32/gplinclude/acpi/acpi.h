/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2009 Erwan Velu - All Rights Reserved
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
 *   Boston MA 02111-1307, USA; either version 2 of the License, or
 *   (at your option) any later version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */

#ifndef ACPI_H
#define ACPI_H
#include <inttypes.h>
#include <stdbool.h>
#include <acpi/structs.h>
#include <acpi/rsdp.h>
#include <acpi/rsdt.h>
#include <acpi/xsdt.h>
#include <acpi/fadt.h>
#include <acpi/madt.h>
#include <acpi/dsdt.h>
#include <acpi/ssdt.h>
#include <acpi/sbst.h>
#include <acpi/ecdt.h>
#include <acpi/facs.h>

enum { ACPI_FOUND, ENO_ACPI, MADT_FOUND, ENO_MADT };

#define MAX_SSDT 128

/* Some other description HEADERS : ACPI doc: 5.2.6*/
#define OEMX "OEMx"
#define SRAR "SRAT"
#define BERT "BERT"
#define BOOT "BOOT"
#define CPEP "CPEP"
#define DBGP "DGBP"
#define DMAR "DMAR"
#define ERST "ERST"
#define ETDT "ETDT"
#define HEST "HEST"
#define HPET "HPET"
#define IBFT "IBFT"
#define MCFG "MCFG"
#define SPCR "SPCR"
#define SPMI "SPMI"
#define TCPA "TCPA"
#define UEFI "UEFI"
#define WAET "WAET"
#define WDAT "WDAT"
#define WDRT "WDRT"
#define WSPT "WSPT"

/* This macro are used to extract ACPI structures 
 * please be careful about the q (interator) naming */
#define cp_struct(dest) memcpy(dest,q,sizeof(*dest)); q+=sizeof(*dest)
#define cp_str_struct(dest) memcpy(dest,q,sizeof(dest)-1); dest[sizeof(dest)-1]=0;q+=sizeof(dest)-1

typedef struct {
    s_rsdp rsdp;
    s_rsdt rsdt;
    s_xsdt xsdt;
    s_fadt fadt;
    s_madt madt;
    s_dsdt dsdt;
    s_ssdt *ssdt[MAX_SSDT];
    uint8_t ssdt_count;
    s_sbst sbst;
    s_ecdt ecdt;
    s_facs facs;
} s_acpi;

int parse_acpi(s_acpi * acpi);
int parse_xsdt(s_acpi * acpi);
void parse_madt(s_acpi * acpi);
int search_rsdp(s_acpi *acpi);
void get_acpi_description_header(uint8_t *q, s_acpi_description_header * adh);
#endif