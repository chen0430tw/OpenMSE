// Copyright (c) Microsoft Corporation. All rights reserved.
// Script for AddToMFC

function OnFinish(selProj, selObj)
{
	try
	{
		//AddATLSupportToProject(selProj) will be called through vsz mechanism.
	}
	catch(e)
	{
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}

