#include <ttobjs.h>
#include <ttload.h>
#include <tttables.h>



int
TT_Get_Num_Tables(TT_Face face)
{
    PFace f;

    f = HANDLE_Face(face);

    return f->numTables;
}



TT_Error
TT_Get_Table_Index(TT_Face face, ULong index,
		   ULong *Tag, ULong *CheckSum, ULong *Offset, ULong *Length)
{
    PFace f;

    f = HANDLE_Face(face);

    if (index >= f->numTables)
	return TT_Err_Invalid_Argument;

    if (Tag)
	*Tag = f->dirTables[index].Tag;
    if (CheckSum)
	*CheckSum = f->dirTables[index].CheckSum;
    if (Offset)
	*Offset = f->dirTables[index].Offset;
    if (Length)
	*Length = f->dirTables[index].Length;

    return TT_Err_Ok;
}



TT_Error
TT_Get_Table_Tag(TT_Face face, ULong Tag,
		ULong *CheckSum, ULong *Offset, ULong *Length)
{
    Long index;
    PFace f;

    f = HANDLE_Face(face);

    index = TT_LookUp_Table(f, Tag);
    if (index == -1)
	return TT_Err_Table_Not_Found;

    return TT_Get_Table_Index(face, (ULong)index, CheckSum, Offset, Length);
}

