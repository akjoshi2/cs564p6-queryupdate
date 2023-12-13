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
	const char* filt;
	Operator operation;
	// Get all attribute info and length
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
		status = attrCat->getInfo(attr->relName, attr->attrName, attrDesc);
		if (status != OK)
		{
			return status;
		}
		int intgr;
		float flt;
		operation = op;
		// Handle types
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
		// If attr is null, unconditional scan should be performed
		strcpy(attrDesc.relName, projNames[0].relName);
		strcpy(attrDesc.attrName, projNames[0].attrName);
		attrDesc.attrOffset = 0;
		attrDesc.attrLen = 0;
		attrDesc.attrType = STRING;
		filt = NULL;
		operation = EQ;
	}
	// Call ScanSelect
	status = ScanSelect(result, projCnt, projDesc, &attrDesc, operation, filt, len);
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
	Status status;
	InsertFileScan rel(result, status);
	if (status != OK)
	{
		return status;
	}
	char outputData[reclen];
	Record outputRec;
	outputRec.data = (void*) outputData;
	outputRec.length = reclen;
	HeapFileScan scan(attrDesc->relName, status);
	if (status != OK)
	{
		return status;
	}
	status = scan.startScan(attrDesc->attrOffset, attrDesc->attrLen, (Datatype) attrDesc->attrType, filter, op);
	if (status != OK)
	{
		return status;
	}
	RID rid;
	Record rec;
	while (scan.scanNext(rid) == OK)
	{
		status = scan.getRecord(rec);
		if (status != OK)
		{
			return status;
		}
		int offset = 0;
		for (int i = 0; i < projCnt; i++)
		{
			memcpy(outputData + offset, (char*)rec.data + projNames[i].attrOffset, projNames[i].attrLen);
			offset += projNames[i].attrLen;
		}
		RID newRid;
		status = rel.insertRecord(outputRec, newRid);
		if (status != OK)
		{
			return status;
		}
	}
	return OK;
}
