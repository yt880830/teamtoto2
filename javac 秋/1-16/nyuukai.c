/************************************/
/*  nyuukai.c                       */
/*    ����o�^�����v���O����        */
/************************************/
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "main.h"
#include "nyuukai.h"

extern int akicode_tbl[ MEMBER_MAX + 1 ];        /* �󂫃R�[�h�\          */
extern int codedata_tbl[ MEMBER_MAX ];           /* �R�[�h�E�f�[�^�Ώƕ\  */
extern struct KEISOKU_TBL kojin_keisoku_tbl;     /* �l�ʌv���f�[�^�\    */

/***********************************/
/* ����o�^����                    */
/*   ���C�����[�`��                */
/*                                 */
/*   �p�����[�^ : �Ȃ�             */
/*   ���^�[��   : 0:OK             */
/*               -1:NG             */
/***********************************/
int nyuukai_touroku( void )
{
    int    ret;             /* ���^�[���R�[�h      */
    int    kaiin_code;      /* �o�^����R�[�h      */
    long   fptr;            /* �t�@�C���|�C���^    */
    char   msg[ 64 ];       /* ���b�Z�[�W�G���A    */

    /* �󂫃R�[�h�\ READ -> NG ? */
    if( (ret = akicode_tbl_read( )) == NG ) {
        return ret;
    }

    /* �󂫃R�[�h���� ? */
    if( akicode_tbl[ 0 ] <= 0 ) {
        printf( "\n �c�O�Ȃ��炽���������o�[�̋󂫂�����܂���\n" );
        ret = OK;
        return ret;
    }

    /* �󂫃R�[�h�m�F */
    sprintf( msg, "\n ����R�[�h�� %d �ł��B��낵���ł���( Y/N )", akicode_tbl[ 1 ] );

    if( (ret = kakunin_input( msg )) == OK ) {
        /* �o�^����R�[�h�ޔ� */
        kaiin_code = akicode_tbl[ 1 ];

        /* �󂫃R�[�h�\�X�V -> OK ? */
        if( (ret = akicode_tbl_update( )) == OK ) {

            /* �v���f�[�^�\�ǉ� -> OK ? */
            if( (ret = keisoku_tbl_add( &fptr, kaiin_code )) == OK ) {

                /* �R�[�h�E�f�[�^�Ώƕ\�X�V */
                ret = codedata_tbl_update( kaiin_code, fptr );
            }
        }
    }

    if( ret == OK ) {
        printf( "\n ����o�^�������I�����܂���" );
    }

    return ret;
}



/************************************/
/* ����o�^����                     */
/*   �󂫃R�[�h�\�@�X�V����         */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int akicode_tbl_update( void )
{
    int     ret;                        /* ���^�[���R�[�h       */
    int     i;                          /* �C���f�b�N�X         */
    int     cnt;                        /* �󂫃R�[�h����       */
    FILE    *fp;                        /* �t�@�C���|�C���^     */
    char    *fname = AKICODE_TBL_NAME;  /* �󂫃R�[�h�\�t�@�C�� */

    /* �󂫃R�[�h�����Z�b�g */
    cnt = akicode_tbl[ 0 ];

    /* �󂫃R�[�h�ҏW */
    for( i = 1; i < cnt; i++ ) {
        if( akicode_tbl[ i + 1 ] == 0 ) {
            break;
        }
        akicode_tbl[ i ] = akicode_tbl[ i + 1 ];
    }

    akicode_tbl[ i ] = 0;

    /* �󂫃R�[�h�����Z�b�g */
    akicode_tbl[ 0 ] = cnt - 1;

    /* �󂫃R�[�h�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n �󂫃R�[�h�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �󂫃R�[�h�\�t�@�C�� WRITE -> 1�ȊO ? */
    if( (ret = fwrite( (char *)akicode_tbl,
               sizeof( int ) * (akicode_tbl[ 0 ] + 1), 1, fp )) != 1 ) {
        printf( "\n �󂫃R�[�h�\�t�@�C�� WRITE �G���[" );
        ret = NG;
    }
    else {
        ret = OK;
    }

    /* �󂫃R�[�h�\�t�@�C�� CLOSE */
    fclose( fp );

    return ret;
}



/************************************/
/* ����o�^����                     */
/*   �v���R�[�h�\�@�ǉ�����         */
/*                                  */
/*   �p�����[�^ : �v���f�[�^�|�C���^*/
/*                �o�^����R�[�h    */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int keisoku_tbl_add( long *fptr, int kaiin_code )
{
    int     ret;                        /* ���^�[���R�[�h         */
    FILE    *fp;                        /* �t�@�C���|�C���^       */
    char    *fname = KEISOKU_TBL_NAME;  /* �v���f�[�^�\�t�@�C��   */

    /* �v���f�[�^�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "a+b" )) == NULL ) {
        printf( "\n �v���f�[�^�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �v���f�[�^�\�t�@�C�� SEEK -> OK �łȂ� ? */
    if( (ret = fseek( fp, 0L, SEEK_END )) != OK ) {
        printf( "\n �v���f�[�^�\�t�@�C�� SEEK �G���[" );
        /* �v���f�[�^�\�t�@�C�� CLOSE */
        fclose( fp );
        return NG;
    }

    /* �t�@�C���|�C���^�擾 */
    *fptr = ftell( fp );

    /* �v���f�[�^�\�N���A */
    kojin_keisoku_tbl = init_kojin_keisoku_tbl();

    /* ����R�[�h�Z�b�g */
    kojin_keisoku_tbl.kaiin_code = kaiin_code;

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



/*****************************************/
/* ����o�^����                     �@�@ */
/*   �R�[�h�E�f�[�^�Ώƕ\�@�X�V����      */
/*                                       */
/*   �p�����[�^ : �o�^����R�[�h         */
/*                �v���f�[�^�|�C���^     */
/*   ���^�[��   : 0:OK                   */
/*               -1:NG                   */
/*****************************************/
static int codedata_tbl_update( int kaiin_code, long fptr )
{
    int     ret;                        /* ���^�[���R�[�h                */
    FILE    *fp;                        /* �t�@�C���|�C���^              */
    char    *fname = CODEDATA_TBL_NAME; /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C��  */

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "r+b" )) == NULL ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� READ -> 1�ȊO ? */
    if( (ret = fread( (char *)codedata_tbl, sizeof( codedata_tbl ), 1, fp ) )
               != 1 ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� READ �G���[" );
        ret = NG;
    }
    else {
        /* �Y���f�[�^�|�C���^�Z�b�g */
        codedata_tbl[ kaiin_code - 1 ] = 
            (int)( (fptr / sizeof( struct KEISOKU_TBL )) + 1 );

        /* �t�@�C���|�C���^��擪�� SEEK -> OK �łȂ� ? */
        if( (ret = fseek( fp, 0L, SEEK_SET )) != OK ) {
            printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� SEEK �G���[" );

            /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
            fclose( fp );
            return NG;
        }

        /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� WRITE -> 1�ȊO ? */
        if( (ret = fwrite( (char *)codedata_tbl, sizeof( codedata_tbl ), 1,
                   fp )) != 1 ) {
            printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� WRITE �G���[" );
            ret = NG;
        }
        else {
            ret = OK;
        }
    }

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
    fclose( fp );

    return ret;
}
