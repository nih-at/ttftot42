ACID_FILES=ac10.acid ag12.acid aj12.acid aj20.acid ak11.acid
CID_FILES =ac10.cid  ag12.cid  aj12.cid  aj20.cid  ak11.cid

pkgdata_DATA=index $(CID_FILES)

EXTRA_DIST=index $(ACID_FILES)

SUFFIXES=.acid .cid

.acid.cid:
	../acidtocid -o $@ $<
