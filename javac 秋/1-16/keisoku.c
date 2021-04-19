/**************************************/
/*  keisoku.c                         */
/*  �@�v���L�^���͏����v���O����      */
/**************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "main.h"
#include "keisoku.h"

extern int akicode_tbl[ MEMBER_MAX + 1 ];                  /* �󂫃R�[�h�\           */
extern int codedata_tbl[ MEMBER_MAX ];                     /* �R�[�h�E�f�[�^�Ώƕ\   */
extern struct KEISOKU_TBL kojin_keisoku_tbl;               /* �l�ʌv���f�[�^�\     */
extern struct KEISOKU_TBL sort_keisoku_tbl[ MEMBER_MAX ];  /* �\�[�g�p�v���f�[�^�\   */


/* ���͌v���f�[�^�e�[�u�� */
static struct  KEISOKU_INPUT   keisoku_indata[ 5 ];

/************************************/
/* �v���L�^���͏���                 */
/*   ���C�����[�`��                 */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
int keisoku_input( void )
{
    int     ret;                /* ���^�[���R�[�h    */
    int     i;                  /* �C���f�b�N�X      */
    int     kaiin_code;         /* �v������R�[�h    */
    int     undou_sisuu;        /* �Z�o�^���w��      */
    char    msg[ 64 ];          /* ���b�Z�[�W�G���A  */
    char    keisoku_date[ 9 ];  /* �v�����t          */

    /* �󂫃R�[�h�\ READ -> NG ? */
    if( (ret = akicode_tbl_read( )) == NG ) {
        return ret;
    }

    /* ����҃`�F�b�N */
    if( akicode_tbl[ 0 ] >= MEMBER_MAX ) {
        printf( "\n ���݁C����҂����܂���" );
        return NG;
    }

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� READ -> NG ? */
    if( (ret = codedata_tbl_read( )) == NG ) {
        return ret;
    }

    /* �v������R�[�h���� */
    kaiin_code_input( &kaiin_code );

    /* �v�����t���� */
    keisoku_date_input( keisoku_date );

    /* �v���l���͏��� */
    for( i = 0; i < 5; i++ ) {
        keisoku_data_input( i );
    }

    /* ���̓f�[�^�\�� */
    keisoku_data_disp( kaiin_code, keisoku_date );

    /* ���͊m�F */
    strcpy( msg, "\n ���͂͂�낵���ł���( Y/N )" );

    /* 'N' OR 'n'���� -> OK �łȂ� ? */
    if( (ret = kakunin_input( msg )) != OK ) {
        return NG;
    }

    /* �v���f�[�^�\ READ -> NG ? */
    if( (ret = kojin_data_read( kaiin_code )) == NG ) {
        return ret;
    }

    /* �v���l�v�Z���� */
    undou_sisuu_keisan( &undou_sisuu );

    /* �v���f�[�^�\ SET */
    kojin_data_update( kaiin_code, keisoku_date, undou_sisuu );

    /* �v�����ʃf�[�^�\�� */
    kojin_data_disp( kaiin_code, "\n ** �v�����ʃf�[�^ **" );

    /* �L�[���͑҂� */
    while( getchar( ) != '\n' ) ;
    printf( "\n ���^�[���L�[�������Ă�������" );
    getchar( );

    /* �v�����ʏ��ʃ\�[�g�\�� */
    ret = keisoku_rank( );

    return ret;
}



/**************************************/
/* �v���L�^���͏���                   */
/*   �R�[�h�E�f�[�^�Ώƕ\�@�Ǎ��ݏ��� */
/*                                    */
/*   �p�����[�^ : �Ȃ�                */
/*   ���^�[��   : 0:OK                */
/*               -1:NG                */
/**************************************/
static int codedata_tbl_read( void )
{
    int     ret;                        /* ���^�[���R�[�h               */
    FILE    *fp;                        /* �t�@�C���|�C���^             */
    char    *fname = CODEDATA_TBL_NAME; /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� */

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� READ -> 1�ȊO ? */
    if( (ret = fread( (char *)codedata_tbl, sizeof( codedata_tbl ), 1, fp ))
               != 1 ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� READ �G���[" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
    fclose( fp );

    return ret;
}



/************************************/
/* �v���L�^���͏���                 */
/*   ����R�[�h���͏���             */
/*                                  */
/*   �p�����[�^ : ���͉���R�[�h    */
/*   ���^�[��   : �Ȃ�              */
/************************************/
static void kaiin_code_input( int *kaiin_code )
{
    int     loop = TRUE;    /* ���[�v�t���O */
    char    work[ 128 ];    /* ���̓��[�N   */

    while( loop ) {
        printf( "\n �v���҂̉���R�[�h����͂��Ă�������" );
        printf( "\n ? " );

        /* ����R�[�h���� */
        work[ 0 ] = '\0';
        scanf( "%s", work );

        /* �j���[�����b�N�E�`�F�b�N -> ���l�ȊO ? */
        if( strspn( work, "1234567890" ) < strlen( work ) ) {
            printf( "\n ���l�ȊO�����͂���܂���" );
            continue;
        }

        /* ���͔͈̓`�F�b�N( 0 < kaiin_code <= MEMBER_MAX ) */
        *kaiin_code = atoi( work );
        if( *kaiin_code > MEMBER_MAX || *kaiin_code <= 0 ) {
            printf( "\n ���̓~�X�ł�" );
            continue;
        }

        /* ����R�[�h�o�^�`�F�b�N -> ���o�^ ? */
        if( codedata_tbl[ *kaiin_code - 1 ] == 0 ) {
            printf( "\n ���̉���R�[�h�͖��o�^�ł�" );
            continue;
        }
        break;
    }
    return;
}



/************************************/
/* �v���L�^���͏���                 */
/*   �v�����t���͏���               */
/*                                  */
/*   �p�����[�^ : ���͓��t          */
/*   ���^�[��   : �Ȃ�              */
/************************************/
static void keisoku_date_input( char *keisoku_date )
{
    int     loop = TRUE;    /* ���[�v�t���O  */
    int     chk_date;       /* ���t���l      */
    char    conv[ 3 ];      /* ���l�ϊ��p    */
    char    work[ 128 ];    /* ���̓��[�N    */

    while( loop ) {
        printf( "\n ���t����͂��Ă�������( YYYYMMDD )" );
        printf( "\n ? " );

        /* ���t���� */
        work[ 0 ] = '\0';
        scanf( "%s", work );

        /* ���͌����`�F�b�N -> 8�ȊO ? */
        if( strlen( work ) != 8 ) {
            printf( "\n ���̓~�X�ł�" );
            continue;
        }

        /* �j���[�����b�N�E�`�F�b�N -> ���l�ȊO ? */
        if( strspn( work, "1234567890" ) < strlen( work ) ) {
            printf( "\n ���l�ȊO�����͂���܂���" );
            continue;
        }

        /* ���`�F�b�N */
        conv[0] = work[4];
        conv[1] = work[5];
        conv[2] = '\0';

        chk_date = atoi( conv );
        if( chk_date > 12 || chk_date < 1 ) {
            printf( "\n ���t( �� )���̓G���[�ł�" );
            continue;
        }

        /* ���`�F�b�N */
        conv[0] = work[6];
        conv[1] = work[7];
        conv[2] = '\0';
        chk_date = atoi( conv );
        if( chk_date > 31 || chk_date < 1 ) {
            printf( "\n ���t( �� )���̓G���[�ł�" );
            continue;
        }

        break;
    }

    /* ���̓f�[�^ �Z�b�g */
    strcpy(keisoku_date, work);
    return;
}



/************************************/
/* �v���L�^���͏���                 */
/*   �v���f�[�^���͏���             */
/*                                  */
/*   �p�����[�^ : �C���f�b�N�X      */
/*   ���^�[��   : �Ȃ�              */
/************************************/
static void keisoku_data_input( int idx )
{
    int     i;                  /* �C���f�b�N�X   */
    int     loop = TRUE;        /* ���[�v�t���O   */
    char    work[ 3 ][ 128 ];   /* ���̓��[�N     */

    while( loop ) {
        printf( "\n �^�� %d �̌v���f�[�^����͂��Ă�������", idx + 1 );
        printf( "\n   ����( 999 )  �Z�b�g( 100 )  ��( 100 )" );
        printf( "\n ? " );

        /* �v���f�[�^���� */
        work[ 0 ][ 0 ] = '\0';
        work[ 1 ][ 0 ] = '\0';
        work[ 2 ][ 0 ] = '\0';
        scanf( "%s %s %s", work[ 0 ], work[ 1 ], work[ 2 ] );

        for( i = 0; i < 3; i++ ) {

            /* ���͌����`�F�b�N -> 3���傫�� ? */
            if( strlen( work[ i ] ) > 3 ) {
                printf( "\n ���̓~�X�ł�" );
                break;
            }

            /* �j���[�����b�N�E�`�F�b�N -> ���l�ȊO ? */
            if( strspn( work[ i ], "1234567890" ) < strlen( work[ i ] ) ) {
                printf( "\n ���l�ȊO�����͂���܂���" );
                break;
            }

            /* ���׈ȊO�̏���`�F�b�N -> 100���傫�� ? */
            if( i != 0 ) {
                if( atoi( work[ i ] ) > 100 ) {
                    printf( "\n ���( 100 )�𒴂��Ă��܂�" );
                    break;
                 }
            }
        }

        if( i < 3 ) {
            continue;
        }

        break;
    }

    /* ���̓f�[�^�e�[�u�� �Z�b�g */
    keisoku_indata[ idx ].huka = atoi( work[ 0 ] );
    keisoku_indata[ idx ].set = atoi( work[ 1 ] );
    keisoku_indata[ idx ].kaisuu = atoi( work[ 2 ] );

    return;
}



/************************************/
/* �v���L�^���͏���                 */
/*   ���͌v���f�[�^�@�\������       */
/*                                  */
/*   �p�����[�^ : ����R�[�h        */
/*                ���t              */
/*   ���^�[��   : �Ȃ�              */
/************************************/
static void keisoku_data_disp( int kaiin_code, char *keisoku_date )
{
    int     i;      /* �C���f�b�N�X      */

    printf( "\n ** ���͌v���l�f�[�^ **" );
    printf( "\n ����R�[�h %3d", kaiin_code );
    printf( "\n ���t       %4.4s-%2.2s-%2.2s", 
        ( keisoku_date + 0 ), ( keisoku_date + 4 ), ( keisoku_date + 6 ) );

    printf( "\n\n �^��   ����  �Z�b�g   ��" );

    for( i = 0; i < 5; i++ ) {
        printf( "\n   %d     %3d    %3d     %3d", i + 1, 
            keisoku_indata[ i ].huka,
            keisoku_indata[ i ].set,
            keisoku_indata[ i ].kaisuu );
    }

    return;
}



/************************************/
/* �v���L�^���͏���                 */
/*   �^���w���v�Z����               */
/*                                  */
/*   �p�����[�^ : �^���w��          */
/*   ���^�[��   : �Ȃ�              */
/************************************/
static void undou_sisuu_keisan( int *undou_sisuu )
{
    int     i;              /* �C���f�b�N�X  */
    double  sisuu;          /* �v�Z���[�N    */
    double  sisuu_total;    /* �v�Z�l���v    */
    static  double  huka_sisuu[ 5 ] = { 0.24, 0.36, 0.52, 1.05, 2.13 };

    sisuu_total = 0.0;
    for( i = 0; i < 5; i++ ) {

        /* �� 1�ȉ� ? */
        if( keisoku_indata[ i ].kaisuu <= 1 ) {
            continue;
        }

        sisuu = huka_sisuu[ i ] * (double)keisoku_indata[ i ].huka * 
               (double)keisoku_indata[ i ].set *
               ((pow( (double)keisoku_indata[ i ].kaisuu, 2.0 )) /
               ((double)keisoku_indata[ i ].kaisuu - 1.0));

        /* �V�O�}(�݌v)�̌v�Z */
        sisuu_total += sisuu;
    }

    /* �������̎Z�o */
    *undou_sisuu = (int)sqrt( sisuu_total );

    return;
}



/************************************/
/* �v���L�^���͏���                 */
/*   �l�v���f�[�^�\�@�X�V����     */
/*                                  */
/*   �p�����[�^ : ����R�[�h        */
/*                ���t              */
/*                �^���w��          */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int kojin_data_update( int kaiin_code, char *keisoku_date, int undou_sisuu )
{
    int     ret;                        /* ���^�[���R�[�h         */
    long    fptr;                       /* �v���f�[�^�|�C���^     */
    FILE    *fp;                        /* �t�@�C���|�C���^       */
    char    *fname = KEISOKU_TBL_NAME;  /* �v���f�[�^�\�t�@�C��   */
    int     i;                          /* �C���f�b�N�X           */

    /* 1��� ? */
    if( kojin_keisoku_tbl.count <= 0 ) {
        strcpy( kojin_keisoku_tbl.first_date, keisoku_date );
        kojin_keisoku_tbl.first_data = undou_sisuu;

        strcpy( kojin_keisoku_tbl.max_date, keisoku_date );
        kojin_keisoku_tbl.max_data = undou_sisuu;
    }
    else {

        /* �ō��L�^ ? */
        if( kojin_keisoku_tbl.max_data < undou_sisuu ) {
            strcpy( kojin_keisoku_tbl.max_date, keisoku_date );
            kojin_keisoku_tbl.max_data = undou_sisuu;
        }
        for (i = sizeof kojin_keisoku_tbl.soku_data / sizeof(int) - 1; i > 0; i-- )
            kojin_keisoku_tbl.soku_data[ i ] = kojin_keisoku_tbl.soku_data[ i - 1 ];
    }

    strcpy( kojin_keisoku_tbl.soku_date, keisoku_date );
    kojin_keisoku_tbl.soku_data[ 0 ] = undou_sisuu;
    kojin_keisoku_tbl.count++;

    /* �v���f�[�^�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "r+b" )) == NULL ) {
        printf( "\n �v���f�[�^�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �Y���f�[�^�|�C���^ �Z�b�g */
    fptr = ( codedata_tbl[ kaiin_code - 1 ] - 1 ) * 
        sizeof( struct KEISOKU_TBL );

    /* �v���f�[�^�\�t�@�C�� SEEK -> OK �łȂ� ? */
    if( (ret = fseek( fp, fptr, SEEK_SET )) != OK ) {
        printf( "\n �v���f�[�^�\�t�@�C�� SEEK �G���[" );

        /* �v���f�[�^�\�t�@�C�� CLOSE */
        fclose( fp );
        return NG;
    }

    /* �v���f�[�^�\�t�@�C�� WRITE -> 1�ȊO ? */
    if( (ret = fwrite( (char *)&kojin_keisoku_tbl, sizeof( kojin_keisoku_tbl ),
               1, fp )) != 1 ) {
        printf( "\n �v���f�[�^�\�t�@�C�� WRITE �G���[" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    /* �v���f�[�^�\�t�@�C�� CLOSE */
    fclose( fp );

    return ret;
}



/************************************/
/* �v���L�^���͏���                 */
/*   �v�����ʏ��ʃ\�[�g�E�\������   */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int keisoku_rank( void )
{
    int     ret;                        /* ���^�[���R�[�h         */
    int     i;                          /* �C���f�b�N�X           */
    FILE    *fp;                        /* �t�@�C���|�C���^       */
    char    *fname = KEISOKU_TBL_NAME;  /* �v���f�[�^�\�t�@�C��   */

    /* �v���f�[�^�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n �v���f�[�^�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    i = 0;
    for( ; ; ) {

        /* �v���f�[�^�\�t�@�C�� READ -> 1�ȊO ? */
        if( (ret = fread( (char *)&sort_keisoku_tbl[ i ],
                   sizeof( struct KEISOKU_TBL ), 1, fp )) != 1 ) {

            /* READ �G���[ ? */
            if( ferror( fp ) != 0 ) {
                printf( "\n �v���f�[�^�\�t�@�C�� READ �G���[" );
                ret = NG;
            }
            else {
                /* �t�@�C�� EOF �łȂ� ? */
                if( feof( fp ) == 0 ) {
                    printf( "\n �v���f�[�^�\�t�@�C�� READ �G���[" );
                    ret = NG;
                }
                else {
                    ret = OK;
                }
            }
        }

        /* READ �G���[ ? */
        if( ret == NG ) {
            break;
        }

        /* �v���f�[�^���� ? */
        if( sort_keisoku_tbl[ i ].count != 0 ) {
            i++;
        }

        /* eof ? */
        if( ret == OK ) {
            break;
        }
    }

    /* �v���f�[�^�\�t�@�C�� CLOSE */
    fclose( fp );

    if( ret == OK ) {
        /* �v���f�[�^�\�[�g */
        keisoku_data_sort( i );
        /* �v���f�[�^�\�[�g���ʕ\�� */
        keisoku_sort_disp( i );
    }

    return ret;
}



/************************************/
/* �v���L�^���͏���                 */
/*   �v�����ʏ��ʃ\�[�g����         */
/*                                  */
/*   �p�����[�^ : �v���f�[�^����    */
/*   ���^�[��   : �Ȃ�              */
/************************************/
static void keisoku_data_sort( int cnt )
{
    int     i;                      /* �C���f�b�N�X      */
    int     j;                      /* �C���f�b�N�X      */
    struct  KEISOKU_TBL  work;      /* �X���b�v�p�G���A  */

    /* �f�[�^�\�[�g */
    for( i = 0; i < cnt - 1; i++ ) {
        for( j = i + 1; j < cnt; j++ ) {
            if( sort_keisoku_tbl[ i ].max_data < 
                sort_keisoku_tbl[ j ].max_data ) {

                work = sort_keisoku_tbl[ i ];
                sort_keisoku_tbl[ i ] = sort_keisoku_tbl[ j ];
                sort_keisoku_tbl[ j ] = work;
            }
        }
    }
    return;
}



/************************************/
/* �v���L�^���͏���                 */
/*   �v�����ʏ��ʕ\������           */
/*                                  */
/*   �p�����[�^ : �v���f�[�^����    */
/*   ���^�[��   : �Ȃ�              */
/************************************/
static void keisoku_sort_disp( int cnt )
{
    int     i;  /* �C���f�b�N�X  */

    printf( "\n\n ** ���ʕ\ **" );
    printf( "\n ���� ����R�[�h �ō��f�[�^     ���t" );

    for( i = 0; i < cnt; i++ ) {

        /* 10�ʂ܂ŕ\������ */
        if( i >= 10 ) {
            break;
        }

        printf( "\n   %2d   %3d         %4d     %4.4s-%2.2s-%2.2s", i + 1,
            sort_keisoku_tbl[ i ].kaiin_code,
            sort_keisoku_tbl[ i ].max_data,
            &sort_keisoku_tbl[ i ].max_date[ 0 ],
            &sort_keisoku_tbl[ i ].max_date[ 4 ],
            &sort_keisoku_tbl[ i ].max_date[ 6 ] );
    }
    return;
}
