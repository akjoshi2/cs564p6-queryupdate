#include "catalog.h"
#include "query.h"


// forward declaration
const Status ScanSelect(const string & result, 
			const int projCnt, 
			const AttrDesc projNames[],
			const AttrDesc *attrDesc, 
			const Operator op, 
			const char *filter,
			const int reclen);

/*
 * Selects records from the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

const Status QU_Select(const string & result, 
		       const int projCnt, 
		       const attrInfo projNames[],
		       const attrInfo *attr, 
		       const Operator op, 
		       const char *attrValue)
{
   // Qu_Select sets up things and then calls ScanSelect to do the actual work
    cout << "Doing QU_Select " << endl;
	Status status;
	AttrDesc attrDesc;
	AttrDesc* projDesc;
	int len = 0;
	projDesc = new AttrDesc[projCnt];
	char* filt;
	for (int i = 0; i < projCnt; i++)
	{
		status = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, projDesc[i]);
		if (status != OK)
		{
			return status;
		}
		len += projDesc[i].attrLen;
	}
	if (attr != NULL)
	{
		int intgr;
		float flt;
		switch (attr->attrType)
		{
			case INTEGER:
				intgr = atoi(attrValue);
				filt = (char*)&intgr;
				break;
			case FLOAT:
				flt = atof(attrValue);
				filt = (char*)&flt;
				break;
			case STRING:
				filt = attrValue;
				break;
		}
	}
	else
	{
		strcpy(attrDesc.relName, projNames[0].relName);
		strcpy(attrDesc.attrName, projNames[0].attrName);
		attrDesc.attrOffset = 0;
		attrDesc.attrLen = 0;
		attrDesc.attrType = STRING;
		filt = NULL;
		op = EQ;
	}
	status = ScanSelect(result, projCnt, projDesc, &attrDesc, op, filt, len)
	if (status != OK)
	{
		return status;
	}
	return OK;
}

#include "stdio.h"
#include "stdlib.h"
const Status ScanSelect(const string & result, 
			const int projCnt, 
			const AttrDesc projNames[],
			const AttrDesc *attrDesc, 
			const Operator op, 
			const char *filter,
			const int reclen)
{
    cout << "Doing HeapFileScan Selection using ScanSelect()" << endl;


}
