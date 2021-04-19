/*******************************************/
/*  main.c                                 */
/*    �A�X���`�b�N�N���u����Ǘ��v���O���� */
/*******************************************/

#include <stdio.h>
#include <string.h>

#include "common.h"
#include "main.h"
#include "nyuukai.h"
#include "keisoku.h"
#include "sakujyo.h"

/***********************************/
/* ���ʃf�[�^                      */
/***********************************/

/* �󂫃R�[�h�\ */
int akicode_tbl[ MEMBER_MAX + 1 ];

/* �R�[�h�E�f�[�^�Ώƕ\ */
int codedata_tbl[ MEMBER_MAX ];

/* �l�ʌv���f�[�^�\ */
struct KEISOKU_TBL kojin_keisoku_tbl;

/*�\�[�g�p�v���f�[�^�\ */
struct KEISOKU_TBL sort_keisoku_tbl[ MEMBER_MAX ];


/***********************************/
/* ����Ǘ����C������              */
/*   ���C�����[�`��                */
/*                                 */
/*   �p�����[�^ : �Ȃ�             */
/*   ���^�[��   : �Ȃ�             */
/***********************************/
int main( void )
{
    int    loop = TRUE;                 /* ���[�v�t���O                  */
    char   work[ 128 ];                 /* ���̓��[�N                    */
    FILE   *fp;                         /* �t�@�C���|�C���^              */
    char   *fname1 = AKICODE_TBL_NAME;  /* �󂫃R�[�h�\�t�@�C��          */
    char   *fname2 = CODEDATA_TBL_NAME; /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C��  */
    int    i;                           /* �C���f�b�N�X                  */

    /* �󂫃R�[�h�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname1, "rb" )) == NULL ) {
        printf( "\n �󂫃R�[�h�\�t�@�C�����쐬���Ă��܂�" );

        /* �󂫃R�[�h�\�t�@�C���쐬 */
        akicode_tbl_create( );
    }
    else {
        /* �󂫃R�[�h�\�t�@�C�� CLOSE */
        fclose( fp );
    }

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname2, "rb" )) == NULL ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�����쐬���Ă��܂�" );

        /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C���쐬 */
        codedata_tbl_create( );
    }
    else {
        /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
        fclose( fp );
    }

    while( loop ) {
        /* �e�[�u�������N���A */
        akicode_tbl[ 0 ] = 0;
        for( i = 0; i < MEMBER_MAX; i++) {
            akicode_tbl[ i + 1 ] = 0;
            codedata_tbl[ i ] = 0;
            sort_keisoku_tbl[ i ] = init_kojin_keisoku_tbl();
        }
        kojin_keisoku_tbl = init_kojin_keisoku_tbl();

        printf( "\n" );
        printf( "\n *******************************************" );
        printf( "\n  �A�X���`�b�N�N���u �����o�[�Ǘ��v���O����" );
        printf( "\n *******************************************" );
        printf( "\n ������I�����Ă�������" );
        printf( "\n 1:����o�^" );
        printf( "\n 2:�v���L�^����" );
        printf( "\n 3:�o�^�폜" );
        printf( "\n E:�I��" );
        printf( "\n ? " );

        /* �����敪���� */
        work[ 0 ] = '\0';
        scanf( "%s", work );

        /* ���͌����`�F�b�N -> 1�ȊO ? */
        if( strlen( work ) != 1 ) {
            printf( "\n ���̓~�X�ł�" );
            continue;
        }

        switch( work[ 0 ] ) {
            case '1':       /* ����o�^     */
                nyuukai_touroku( );
                break;

            case '2':       /* �v���L�^���� */
                keisoku_input( );
                break;

            case '3':       /* �o�^�폜     */
                touroku_sakujyo( );
                break;

            case 'e':       /* �I��         */
            case 'E':
                loop = FALSE;
                break;

            default:
                printf( "\n ���̓~�X�ł�" );
                break;
        }
    }
    return OK;
}



/************************************/
/* ����Ǘ����C������               */
/*   �󂫃R�[�h�\�@�쐬����         */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int akicode_tbl_create( void )
{
    int     ret;                        /* ���^�[���R�[�h        */
    int     i;                          /* �C���f�b�N�X          */
    FILE    *fp;                        /* �t�@�C���|�C���^      */
    char    *fname = AKICODE_TBL_NAME;  /* �󂫃R�[�h�\�t�@�C��  */

    /* �󂫃R�[�h�ҏW */
    akicode_tbl[ 0 ] = MEMBER_MAX;
    for( i = 1; i < MEMBER_MAX + 1; i++ ) {
        akicode_tbl[ i ] = i;
    }

    /* �󂫃R�[�h�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n �󂫃R�[�h�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �󂫃R�[�h�\�t�@�C�� WRITE -> 1�ȊO ? */
    if( (ret = fwrite( (char *)akicode_tbl, sizeof( akicode_tbl ), 1, fp ) )
               != 1 ) {
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
/* ����Ǘ����C������               */
/*   �R�[�h�E�f�[�^�Ώƕ\�@�쐬���� */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int codedata_tbl_create( void )
{
    int     ret;                            /* ���^�[���R�[�h                */
    FILE    *fp;                            /* �t�@�C���|�C���^              */
    char    *fname = CODEDATA_TBL_NAME;     /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C��  */
    int     i;                              /* �C���f�b�N�X                  */

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    for( i = 0; i < MEMBER_MAX; i++ )
        codedata_tbl[ i ] = 0;

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� WRITE -> 1�ȊO ? */
    if( (ret = fwrite( (char *)codedata_tbl, sizeof( codedata_tbl ), 1, fp ) )
               != 1 ) {
        /* WRITE �G���[ */
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� WRITE �G���[" );
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
/* ���ʃ��[�`��                     */
/************************************/


/************************************/
/* ���ʃ��[�`��                     */
/*   �󂫃R�[�h�\�@�Ǎ��ݏ���       */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
int akicode_tbl_read( void )
{
    int     ret;                         /* ���^�[���R�[�h         */
    int     i;                           /* �C���f�b�N�X           */
    FILE    *fp;                         /* �t�@�C���|�C���^       */
    char    *fname = AKICODE_TBL_NAME;   /* �󂫃R�[�h�\�t�@�C��   */

    /* �󂫃R�[�h�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n �󂫃R�[�h�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    for( i = 0; i < MEMBER_MAX + 1; i++ ) {
        /* �󂫃R�[�h�\�t�@�C�� READ -> 1�ȊO ? */
        if( (ret = fread( (char *)&akicode_tbl[ i ], sizeof( int ), 1, fp ) )
                   != 1 ) {
            /* READ�G���[���� ? */
            if( ferror( fp ) != 0 ) {
                printf( "\n �󂫃R�[�h�\�t�@�C�� READ �G���[" );
                ret = NG;
            }
            else {
                /* �t�@�C�� EOF �łȂ�? */
                if( feof( fp ) == 0 ) {
                    printf( "\n �󂫃R�[�h�\�t�@�C�� READ �G���[" );
                    ret = NG;
                }
                else {
                    ret = OK;
                }
            }
            break;
        }
    }

    /* �󂫃R�[�h�\�t�@�C�� CLOSE */
    fclose( fp );

    return ret;
}



/***********************************/
/* ���ʃ��[�`��                    */
/*   ���s�m�F���͏���              */
/*                                 */
/*   �p�����[�^ :�m�F���b�Z�[�W    */
/*   ���^�[��   : 0:OK             */
/*               -1:NG             */
/***********************************/
int kakunin_input( char *msg )
{
    int     ret;            /* ���^�[���R�[�h  */
    int     loop = TRUE;    /* ���[�v�t���O    */
    char    work[ 128 ];    /* ���̓��[�N      */

    while( loop ) {
        /* �m�F�\�� */
        printf( msg );
        printf( "\n ? " );

        /* Y/N���� */
        work[ 0 ] = '\0';
        scanf( "%s", work );

        /* ���͌����`�F�b�N -> 1�ȊO ? */
        if( strlen( work ) != 1 ) {
            printf( "\n ���̓~�X�ł�" );
            continue;
        }

        switch( work[ 0 ] ) {
            case 'Y':   /* Yes */
            case 'y':
                ret  = OK;
                loop = FALSE;
                break;

            case 'N':   /* No */
            case 'n':
                ret  = NG;
                loop = FALSE;
                break;

            default:
                printf( "\n ���̓~�X�ł�" );
                break;
        }
    }

    return ret;
}



/************************************/
/* ���ʃ��[�`��                     */
/*   �l�v���f�[�^�\�@�Ǎ��ݏ���   */
/*                                  */
/*   �p�����[�^ : ����R�[�h        */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
int kojin_data_read( int kaiin_code )
{
    int     ret;                        /* ���^�[���R�[�h                 */
    FILE    *fp;                        /* �v���f�[�^�\�t�@�C���|�C���^   */
    long    fptr;                       /* �v���f�[�^�|�C���^             */
    char    *fname = KEISOKU_TBL_NAME;  /* �v���f�[�^�\�t�@�C��           */

    /* �v���f�[�^�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n �v���f�[�^�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �Y���f�[�^�|�C���^�Z�b�g */
    fptr = ( codedata_tbl[ kaiin_code - 1 ] - 1 ) *
             sizeof( struct KEISOKU_TBL );

    /* �v���f�[�^�\�t�@�C����Ώۂ̈ʒu�܂� SEEK -> OK ? */
    if( (ret = fseek( fp, fptr, SEEK_SET )) != OK ) {
        printf( "\n �v���f�[�^�\�t�@�C�� SEEK �G���[" );

        /* �v���f�[�^�\�t�@�C�� CLOSE */
        fclose( fp );
        return NG;
    }

    /* �v���f�[�^�\�t�@�C�� READ -> 1�ȊO ? */
    if( (ret = fread( (char *)&kojin_keisoku_tbl, sizeof( kojin_keisoku_tbl ),
               1, fp )) != 1 ) {
        printf( "\n �v���f�[�^�\ READ �G���[" );
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
/* ���ʃ��[�`��                     */
/*   �l�v���f�[�^�@�\������       */
/*                                  */
/*   �p�����[�^ : ����R�[�h        */
/*                �\�����b�Z�[�W    */
/*   ���^�[��   : �Ȃ�              */
/************************************/
void kojin_data_disp( int kaiin_code, char *msg )
{
    printf( msg );
    printf( "\n ����R�[�h %3d", kaiin_code );

    if( kojin_keisoku_tbl.count != 0 ) {
        printf( "\n\n �v����    ������t  �f�[�^    " );
        printf( " �ō����t  �f�[�^     �ŐV���t  �f�[�^" );

        printf( "\n    %3d", kojin_keisoku_tbl.count );

        printf( "    %4.4s-%2.2s-%2.2s",
            &kojin_keisoku_tbl.first_date[ 0 ],
            &kojin_keisoku_tbl.first_date[ 4 ],
            &kojin_keisoku_tbl.first_date[ 6 ] );

        printf( "   %4d", kojin_keisoku_tbl.first_data );

        printf( "    %4.4s-%2.2s-%2.2s",
            &kojin_keisoku_tbl.max_date[ 0 ],
            &kojin_keisoku_tbl.max_date[ 4 ],
            &kojin_keisoku_tbl.max_date[ 6 ] );

        printf( "   %4d", kojin_keisoku_tbl.max_data );

        printf( "    %4.4s-%2.2s-%2.2s",
            &kojin_keisoku_tbl.soku_date[ 0 ],
            &kojin_keisoku_tbl.soku_date[ 4 ],
            &kojin_keisoku_tbl.soku_date[ 6 ] );

        printf( "   %4d", kojin_keisoku_tbl.soku_data[ 0 ] );

        printf( "\n\n    1��O   2��O   3��O   4��O   ");
        printf( "5��O   6��O   7��O   8��O   9��O" );

        printf( "\n " );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 1 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 2 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 3 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 4 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 5 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 6 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 7 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 8 ] );
        printf( "    %4d", kojin_keisoku_tbl.soku_data[ 9 ] );
    }
    else {
        printf( "    �v���f�[�^������܂���" );
    return;
    }
    return;
}



/************************************/
/* ���ʃ��[�`��                     */
/*   �v���f�[�^�@����������         */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : �v���f�[�^        */
/************************************/
struct KEISOKU_TBL init_kojin_keisoku_tbl( void )
{
    static struct KEISOKU_TBL tbl = {
        0, 0, "        ", 0, "        ", 0, "        ", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    return tbl;
}
