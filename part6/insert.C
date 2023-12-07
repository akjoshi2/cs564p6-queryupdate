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
	AttrDesc* allAttrs;
	InsertFileScan resultRel(result, status);
    if (status != OK) { return status; }
	status = attrCat->getRelInfo(relation, attrCnt, allAttrs);
}

