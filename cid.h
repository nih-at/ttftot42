#ifndef _HAD_CID_H
#define _HAD_CID_H

/* command codes */

#define C_none	1      /* no active code */
#define C_N	0x0000 /* not encoded */
#define C_O	0x2000 /* range of consecutive codes */
#define C_OV	0x4000 /* range of consecutive codes, vertical */
#define C_L	0x6000 /* list of codes */
#define C_LV	0x8000 /* list of codes, vertical */
#define C_A	0xa000 /* list of alternatives */
#define C_AV	0xc000 /* list of alternatives, vertical */
#define C_AM	0xe000 /* list of alternatives, mixed */

/* decode flags */

#define CID_VERT  0x001
#define CID_ALT   0x002
#define CID_NENC  0x004
#define CID_EOF   0x008



struct cid {
    char *registry, *ordering;
    int supplement;

    int v_major, v_minor;
    int nsupl;
    int *supl_ncid;
    int ncmap;
    struct cid_cmap *cmap;
};

struct cid_feature {
    unsigned long script, language, feature;
};

struct cid_cmap {
    int pid, eid;
    struct cid_feature vert;
    int nfeature;
    struct cid_feature *feature;

    struct cid_code *code;
};

struct cid_code {
    int size;
    int ndata;
    int nchar;
    unsigned short *data;

    int cmd, idx, len, state;
};



struct cid *cid_new(void);
void cid_free(struct cid *cid);
struct cid_code *cid_code_new(void);

struct cid *cid_read(char *fname);
struct cid *cid_aread(char *fname);
int cid_write(char *fname, struct cid *cid);
int cid_awrite(char *fname, struct cid *cid);

int cid_decode_init(struct cid_code *code);
int cid_decode(struct cid_code *code, int *value);

#endif /* cid.h */
