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
    struct cid_feature **feature;

    struct cid_code *code;
};

struct cid_code {
    int size;
    int len;
    int nchar;
    unsigned short *data;

    /* state */
};
