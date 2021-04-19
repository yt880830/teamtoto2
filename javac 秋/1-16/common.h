/************************************/
/*  common.h                        */
/*    ���ʃw�b�_�t�@�C��            */
/************************************/

#define MEMBER_MAX          200             /* �����o�[�� MAX                   */
#define AKICODE_TBL_NAME    "akicode.tbl"   /* �󂫃R�[�h�\�t�@�C����           */
#define CODEDATA_TBL_NAME   "codedata.tbl"  /* �R�[�h�E�f�[�^�Ώƕ\�t�@�C����   */
#define KEISOKU_TBL_NAME    "keisoku.tbl"   /* �v���f�[�^�\�t�@�C����           */

#define  TRUE          1    /* �^            */
#define  FALSE         0    /* �U            */
#define  OK            0    /* ����          */
#define  CANCEL        1    /* �������~      */
#define  NG           -1    /* �ُ�          */

/* �v���f�[�^�e�[�u�� */
struct   KEISOKU_TBL {
    int  kaiin_code;       /* ����R�[�h     */
    int  count;            /* �v����       */
    char first_date[ 9 ];  /* ������t       */
    int  first_data;       /* ����f�[�^     */
    char max_date[ 9 ];    /* �ō��L�^���t   */
    int  max_data;         /* �ō��L�^�f�[�^ */
    char soku_date[ 9 ];   /* �ŐV�v�����t   */
    int  soku_data[ 10 ];  /* �v���f�[�^     */
};

/* ���͌v���f�[�^ */
struct   KEISOKU_INPUT {
    int  huka;             /* ����   */
    int  set;              /* �Z�b�g */
    int  kaisuu;           /* ��   */
};
