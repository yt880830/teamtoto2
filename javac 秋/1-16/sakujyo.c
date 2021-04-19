/************************************/
/*  sakujyo.c                       */
/*    �o�^�폜�����v���O����        */
/************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "main.h"
#include "sakujyo.h"

extern int akicode_tbl[ MEMBER_MAX + 1 ];      /* �󂫃R�[�h�\           */
extern int codedata_tbl[ MEMBER_MAX ];         /* �R�[�h�E�f�[�^�Ώƕ\   */
extern struct KEISOKU_TBL kojin_keisoku_tbl;   /* �l�ʌv���f�[�^�\     */

/************************************/
/* �o�^�폜����                     */
/*   ���C�����[�`��                 */
/*                                  */
/*   �p�����[�^ : �Ȃ�              */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
int touroku_sakujyo( void )
{
    int     ret;            /* ���^�[���R�[�h   */
    int     loop = TRUE;    /* ���[�v�t���O     */
    int     kaiin_code;     /* �폜����R�[�h   */
    char    work[ 128 ];    /* ���̓��[�N       */

    while( loop ) {
        printf( "\n �މ�҂̉���R�[�h����͂��Ă�������" );
        printf( "\n ? " );

        /* ����R�[�h���� */
        work[ 0 ] = '\0';
        scanf( "%s", work );

        /* �j���[�����b�N�E�`�F�b�N -> ���l�ȊO ? */
        if( strspn( work, "1234567890" ) < strlen( work ) ) {
            printf( "\n ���l�ȊO�����͂���܂���" );
            continue;
        }

        /* ���͔͈̓`�F�b�N -> ( 0 < kaiin_code <= MEMBER_MAX ) ? */
        kaiin_code = atoi( work );
        if( kaiin_code > MEMBER_MAX || kaiin_code <= 0 ) {
            printf( "\n ���̓~�X�ł�" );
            continue;
        }

        /* �R�[�h�E�f�[�^�Ώƕ\�폜 -> OK ? */
        if( (ret = codedata_tbl_delete( kaiin_code )) == OK ) {
            /* �󂫃R�[�h�\�ǉ� -> OK ? */
            if( (ret = akicode_tbl_add( kaiin_code )) == OK ) {
                /* ���C���ɖ߂� */
                loop = FALSE;
            }
        }
        else {
            /* ���C���ɖ߂� */
            loop = FALSE;
        }
    }

    if( ret == OK ) {
        printf( "\n ����o�^�폜�������I�����܂���" );
    }

    return ret;
}



/************************************/
/* �o�^�폜����                     */
/*   �R�[�h�E�f�[�^�Ώƕ\�@�폜���� */
/*                                  */
/*   �p�����[�^ : �폜����R�[�h    */
/*   ���^�[��   : 0:OK              */
/*                1:CANCEL          */
/*               -1:NG              */
/************************************/
static int codedata_tbl_delete( int kaiin_code )
{
    int     ret;                        /* ���^�[���R�[�h                */
    int     i;                          /* �C���f�b�N�X                  */
    char    msg[ 64 ];                  /* ���b�Z�[�W�G���A              */
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
        /* READ �G���[ */
        printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� READ �G���[" );

        /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
        fclose( fp );
        return NG;
    }

    /* ����o�^�`�F�b�N -> ���o�^ ? */
    if( codedata_tbl[ kaiin_code - 1 ] == 0 ) {
        printf( "\n ���̉���R�[�h�͖��o�^�ł�" );

        /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
        fclose( fp );
        return CANCEL;
    }

    /* �v���f�[�^�\ READ -> NG ? */
    if( (ret = kojin_data_read( kaiin_code )) == NG ) {
        /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
        fclose( fp );
        return ret;
    }

    /* �v���f�[�^�\�� */
    kojin_data_disp( kaiin_code, "\n ** �폜�f�[�^ **" );

    /* �v���f�[�^�\�폜�m�F */
    sprintf( msg, "\n\n ��̃f�[�^���폜���܂��B��낵���ł���( Y/N )" );

    if( (ret = kakunin_input( msg )) == OK ) {

        /* �v���f�[�^�\�폜 -> OK ? */
        if( (ret = kojin_data_delete( kaiin_code )) == OK ) {

            /* �v���f�[�^�|�C���^�X�V */
            for( i = 0; i < MEMBER_MAX; i++ ) {
                if( codedata_tbl[ i ] > codedata_tbl[ kaiin_code - 1 ] ) {
                    codedata_tbl[ i ]--;
                }
            }

            /* �v���f�[�^�|�C���^�N���A */
            codedata_tbl[ kaiin_code - 1 ] = 0;

            /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C���̐擪�ʒu�� SEEK -> OK ? */
            if( (ret = fseek( fp, 0L, SEEK_SET )) == OK ) {

                /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� WRITE -> 1�ȊO ? */
                if( (ret = fwrite( (char *)codedata_tbl,
                           sizeof( codedata_tbl ),1, fp )) != 1 ) {
                    printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� WRITE �G���[" );
                    ret = NG;
                }
                else {
                    ret = OK;
                }
            }
            else {
                printf( "\n �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� SEEK �G���[" );
                ret = NG;
            }
        }
    }
    else {
        /* �������~ */
        ret = CANCEL;
    }

    /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C�� CLOSE */
    fclose( fp );

    return ret;
}



/************************************/
/* �o�^�폜����                     */
/*   �l�v���f�[�^�\�@�폜����     */
/*                                  */
/*   �p�����[�^ : �폜����R�[�h    */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int kojin_data_delete( int kaiin_code )
{
    int     ret;                         /* ���^�[���R�[�h                */
    int     i;                           /* �C���f�b�N�X                  */
    FILE    *fp;                         /* �v���f�[�^�\�t�@�C���|�C���^  */
    FILE    *tmp;                        /* �e���|�����t�@�C���|�C���^    */
    char    *fname = KEISOKU_TBL_NAME;   /* �v���f�[�^�\�t�@�C��          */
    char    *tmpfl = "keisoku.tmp";      /* �e���|�����t�@�C��            */

    /* �v���f�[�^�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "rb" )) == NULL ) {
        printf( "\n �v���f�[�^�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �e���|�����t�@�C�� OPEN -> NULL ? */
    if( (tmp = fopen( tmpfl, "w+b" )) == NULL ) {
        printf( "\n �e���|�����t�@�C�� OPEN �G���[" );
        fclose( fp );
        return NG;
    }

    i = 0;
    for( ; ; ) {

        /* �v���f�[�^�\�t�@�C�� READ -> 1�ȊO ? */
        if( (ret = fread( (char *)&kojin_keisoku_tbl,
                   sizeof( kojin_keisoku_tbl ), 1, fp )) != 1 ) {

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
            break;
        }

        /* �폜�f�[�^ ? */
        if( kaiin_code == kojin_keisoku_tbl.kaiin_code ) {
            continue;
        }

        /* �e���|�����t�@�C�� WRITE -> 1�ȊO ? */
        if( (ret = fwrite( (char *)&kojin_keisoku_tbl,
                   sizeof( kojin_keisoku_tbl ), 1, tmp )) != 1 ) {
            printf( "\n �v���f�[�^�\�t�@�C�� WRITE �G���[" );
            ret = NG;
            break;
        }
        i++;
    }

    /* �e���|�����t�@�C�� CLOSE */
    fclose( tmp );

    /* �v���f�[�^�\�t�@�C�� CLOSE */
    fclose( fp );

    /* ����I�� ? */
    if( ret == OK ) {

        /* �v���f�[�^�\�t�@�C�� DELETE -> 0�ȊO ? */
        if( (ret = remove( fname )) != 0 ) {
            printf( "\n �v���f�[�^�\�t�@�C���폜�G���[" );
            ret = NG;
        }
        else {
            /* �����݃f�[�^���� ? */
            if( i > 0 ) {

                /* �e���|�����t�@�C�����v���f�[�^�\�t�@�C���Ƀ��l�[������ -> 0�ȊO ? */
                if( (ret = rename( tmpfl, fname )) != 0 ) {
                    printf( "\n �v���f�[�^�\�t�@�C�����l�[���G���[" );
                    ret = NG;
                }
            }
            else {
                /* �e���|�����t�@�C���폜 */
                remove( tmpfl );
            }
        }
    }
    else {
        /* �e���|�����t�@�C���폜 */
        remove( tmpfl );
    }

    return ret;
}



/************************************/
/* �o�^�폜����                     */
/*   �󂫃R�[�h�\�@�ǉ�����         */
/*                                  */
/*   �p�����[�^ : �폜����R�[�h    */
/*   ���^�[��   : 0:OK              */
/*               -1:NG              */
/************************************/
static int akicode_tbl_add( int kaiin_code )
{
    int     ret;                        /* ���^�[���R�[�h        */
    int     cnt;                        /* �󂫃R�[�h����        */
    FILE    *fp;                        /* �t�@�C���|�C���^      */
    char    *fname = AKICODE_TBL_NAME;  /* �󂫃R�[�h�\�t�@�C��  */

    /* �󂫃R�[�h�\ READ -> NG ? */
    if( (ret = akicode_tbl_read( )) == NG ) {
        return ret;
    }

    /* �󂫃R�[�h�����Z�b�g */
    cnt = akicode_tbl[ 0 ];

    /* �󂫃R�[�h�e�[�u���Z�b�g */
    akicode_tbl[ cnt + 1 ] = kaiin_code;

    /* �󂫃R�[�h�����Z�b�g */
    akicode_tbl[ 0 ] = cnt + 1;

    /* �󂫃R�[�h�\�t�@�C�� OPEN -> NULL ? */
    if( (fp = fopen( fname, "w+b" )) == NULL ) {
        printf( "\n �󂫃R�[�h�\�t�@�C�� OPEN �G���[" );
        return NG;
    }

    /* �󂫃R�[�h�\�t�@�C�� WRITE -> 1�ȊO ? */
    if( (ret = fwrite( (char *)akicode_tbl,
               sizeof( int ) * ( akicode_tbl[ 0 ] + 1 ), 1, fp )) != 1 ) {
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
