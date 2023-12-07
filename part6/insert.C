#include "catalog.h"
#include "query.h"


/*
 * Inserts a record into the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Insert(const string & relation, 
	const int attrCnt, 
	const attrInfo attrList[])
{
	Status status;
	int relAttrCnt;
	AttrDesc* relAttrs;
	InsertFileScan resultRel(result, status);
    if (status != OK)
	{
		return status;
	}
	status = attrCat->getRelInfo(relation, relAttrCnt, relAttrs);
	if (status != OK)
	{ 
		return status;
	}
	if (relAttrCnt)
}

