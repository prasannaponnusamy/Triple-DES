#include<iostream>
#include<cstdlib>
#include<cstdint>
using namespace std;
#define LB32   0x00000001 //32 B?T MASKELEME sol bit 32
#define LB64   0x0000000000000001 //64 B?T MASKELEME sol bit 64
#define L64_MASK    0x00000000ffffffff   //S?METR? sol bit simetri alma
#define H64_MASK    0xffffffff00000000	//S?METR? son hal

static char IP[] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

static char PI[] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

static char E[] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

/* Post S-Box permutation */
static char P[] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};

/* The S-Box tablosu */
static char S[8][64] = {{
    /* S1 */
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},{
    /* S2 */
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},{
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},{
    /* S4 */
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},{
    /* S5 */
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},{
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},{
    /* S7 */
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},{
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}};

static char PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

static char PC2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static char iteration_shift[] = {
 /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
    1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1
};


uint64_t des(uint64_t input, uint64_t key,   char mode) {


    int i, j;

    /* 8 bit */
    char satir, sutun;

    /* 28 bits */
    uint32_t C                  = 0;
    uint32_t D                  = 0;

    /* 32 bit */
    uint32_t L                  = 0;
    uint32_t R                  = 0;
    uint32_t s_output           = 0;
    uint32_t f_function_res     = 0;
    uint32_t temp               = 0;

    /* 48 bit */
    uint64_t sub_key[16]        = {0};
    uint64_t s_input            = 0;

    /* 56 bit */
    uint64_t permuted_choice_1  = 0;
    uint64_t permuted_choice_2  = 0;

    /* 64 bit */
    uint64_t init_perm_res      = 0;
    uint64_t inv_init_perm_res  = 0;
    uint64_t pre_output         = 0;

    /* initial permutation */
    for (i = 0; i < 64; i++) {

        init_perm_res <<= 1;//girilen de?eri 1 bit yana kayd?rd?m.
        init_perm_res |= (input >> (64-IP[i])) & LB64; //Initial Permutation tablosunda bit kayd?rma yap?p or kap?s?na sokup
        //|=i?lem ile tekrar or kap?s?na girdi. kontrol i?in



    }
    // printf ("ilk karistirma(girilen): %llx\n", init_perm_res);
    L = (uint32_t) (init_perm_res >> 32) & L64_MASK;// kar?st?rd?k ve 64 biti 32 bit 32 bit ay?rd?k.
      // printf ("SOL 32 Bit: %llx\n",L);
    R = (uint32_t) init_perm_res & L64_MASK;  // sag taraf durcak.
       // printf ("SAG 32 Bit: %llx\n",R);
    /* initial key karistirma */
    for (i = 0; i < 56; i++) {

        permuted_choice_1 <<= 1;//
        permuted_choice_1 |= (key >> (64-PC1[i])) & LB64;//64 bitten 56 bite anahtar? d???r?p kontrollü yapt?k.
		 //printf ("ilk karistirma(anahtar): %llx\n", permuted_choice_1);
    }

    C = (uint32_t) ((permuted_choice_1 >> 28) & 0x000000000fffffff);//kar?san tablodaki bitler 28 sat?r saga kayd?.
    D = (uint32_t) (permuted_choice_1 & 0x000000000fffffff);//ve kontrol yapt?k.
   //  printf ("SOL 56 Bit: %llx\n",C);
   //  printf ("SAG 56 Bit: %llx\n",D);
    /* 16 tane anahtar olusturma*/
    for (i = 0; i< 16; i++) {


        // shift Ci and Di
        for (j = 0; j < iteration_shift[i]; j++) { //shift tablosundaki index kadar d?nd?r?p bit kayd?r?p XOR yapt?k.

            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);

        }
    //	printf ("%d:SOL 56 Bit(SHIFT): %llx\n",i,C);
     //	printf ("%d:SAG 56 Bit(SHIFT): %llx\n",i,D);
        /*uras? da, bir ?stte ?retilen 56 bitlik shifted key i?inden 48 bitlik anahtar ?retimi i?indir.
		Asl?nda burada hem s?k??t?rma hem de kar??t?rma (permutasyon) yap?lm?? olur.
		?rne?in, key?in ilk biti 14. s?rayla de?i?tirilir. ?kinci biti 17.s?rayla, gibi..*/
        permuted_choice_2 = 0;
        permuted_choice_2 = (((uint64_t) C) << 28) | (uint64_t) D ;//kar?san tablodaki bitler 28 sat?r saga kayd?.

        sub_key[i] = 0;
        //48 biti 56 bite c?kard?k.(s?k??t?rma)
        for (j = 0; j < 48; j++) {

            sub_key[i] <<= 1;//32+32
            sub_key[i] |= (permuted_choice_2 >> (56-PC2[j])) & LB64;//anahtar?n son hali 64 bit oldu.

        }

    }


    //16 kere d?nd?r?p
    for (i = 0; i < 16; i++) {


        s_input = 0;

        for (j = 0; j< 48; j++) {

            s_input <<= 1;
            s_input |= (uint64_t) ((R >> (32-E[j])) & LB32);// kar?st?rd?g?m?z girilen de?eri 32 bit kayd?r?p xor a soktuk.

        }

        /*
        Burada, 48 bitlik geni?letilmi? d?z text (expanded RPT of 48 bit) ile 48 bitlik anahtar
		(compressed key of 48 bit) aras?nda bitwise XOR i?lemi yap?l?yor. Sonu? olarak yine 48 bitlik bir
		metin ortaya ??kacakt?r.

       if (mode == 'd') {
            // decryption
          //  s_input = s_input ^ sub_key[15-i];

       // } else {
            // encryption


       // }

        /* S-Box tablosu */
       /* Bir ?nceki i?lemle XOR?lanm?? metin (XORed RPT) ( text ile keyin birle?tirilmesi) S box?a verilir.
	   Burada metin (48 bit), her biri 6 bitten olu?an 8 blo?a b?l?n?r. Her blok i?in ayr? bir Sbox tablosu bulunur.
	   Bu sebeple de a?a??da 8 adet Sbox tablosu g?receksiniz. Sbox?lar 16 s?tun, 4 sat?rdan olu?ur. 0 ile 15 aras?nda de?er
	    al?r. Ve her SBox 4 bitlik ??k?? verir. B?t?n SBox?lar?n g?revi bitti?inde sonu? olarak (4*8) 32 bit d?necektir.
		(Sbox RPT) Bu 6 bitlik verinin 1. ve 6. verisi sat?r, 2.3.4.5 bitleri ise Sbox ?zerindeki s?t?nlar? g?sterir.
		Bunlar?n kesi?ti?i nokta da d?n?? de?erini olu?turacakt?r. B?ylece her sbox??n 4 bitlik ??k??? olacakt?r.*/

		  if (mode == 'd') {
            //decryption
            s_input = s_input ^ sub_key[15-i];

        	} else {
            // encryption
            s_input = s_input ^ sub_key[i]; //kar?st?r?lan anahtar ve key son halimizi ald?.

        	}


        for (j = 0; j < 8; j++) {


            satir = (char) ((s_input & (0x0000840000000000 >> 6*j)) >> 42-6*j);
            satir = (satir >> 4) | satir & 0x01;

            sutun = (char) ((s_input & (0x0000780000000000 >> 6*j)) >> 43-6*j);

            s_output <<= 4;
            s_output |= (uint32_t) (S[j][16*satir + sutun] & 0x0f);

        }

        f_function_res = 0;

        for (j = 0; j < 32; j++) {

            f_function_res <<= 1;
            f_function_res |= (s_output >> (32 - P[j])) & LB32;

        }

        temp = R;
        R = L ^ f_function_res;
        L = temp;
        //printf ("%d:SAG: %llx  SOL: %llx\n",j, R,L);
    }

    pre_output = (((uint64_t) R) << 32) | (uint64_t) L;

    /* inverse initial permutation */
    for (i = 0; i < 64; i++) {

        inv_init_perm_res <<= 1;
        inv_init_perm_res |= (pre_output >> (64-PI[i])) & LB64;

    }

    return inv_init_perm_res;

}

int main(int argc, const char * argv[]) {


     int i;

    uint64_t input = 0x9474B8E8C73BCA7D;
   	uint64_t key[3] ={0x9474B8E8C73BCA7D, 0x9474B8E8C73BCA7C, 0x9474B8E8C73BC97D};
    uint64_t result = input;

	cout<<"\nEcrypt DES & 3-DES\n----------------------\n";
    cout<<"Input: %016llx\n", input;
    result = des(input, key[0], 'e');
    cout<<"Des: %016llx\n", result;
    result = des(result, key[1], 'd');
    //cout<<"Des: %016llx\n", result;
    result = des(result, key[2], 'e');
    cout<<"3Des: %016llx\n", result;

    cout<<"\nDecrypt DES & 3-DES\n----------------------\n";
    result = des(result, key[2], 'd');
    cout<<"Des: %016llx\n", result;
    result = des(result, key[1], 'e');
    //cout<<"Des: %016llx\n", result;
    result = des(result, key[0], 'd');
    cout<<"3Des: %016llx\n", result;

 /*
     * TESTING IMPLEMENTATION OF DES
     * Ronald L. Rivest
     * X0:  9474B8E8C73BCA7D
     * X16: 1B1A2DDB4C642438
     *
     * OUTPUT:
     * E: 8da744e0c94e5e17
     * D: 0cdb25e3ba3c6d79
     * E: 4784c4ba5006081f
     * D: 1cf1fc126f2ef842
     * E: e4be250042098d13
     * D: 7bfc5dc6adb5797c
     * E: 1ab3b4d82082fb28
     * D: c1576a14de707097
     * E: 739b68cd2e26782a
     * D: 2a59f0c464506edb
     * E: a5c39d4251f0a81e
     * D: 7239ac9a6107ddb1
     * E: 070cac8590241233
     * D: 78f87b6e3dfecf61
     * E: 95ec2578c2c433f0
     * D: 1b1a2ddb4c642438  <-- X16


    */



    exit(0);

}
