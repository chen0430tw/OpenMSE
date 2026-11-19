//=======================================================================
// Microsoft SQL Server for Windows CE
//
// Microsoft Confidential.
//
// Copyright (c) 1994-2001 Microsoft Corporation.
//
// Component: Common files
// 
// File:  ssceerr.h
//
// File Comments:	error codes for all of our stuff.
//=======================================================================

#ifndef SSCEERR_INCLUDED
#define SSCEERR_INCLUDED

//	HRESULTS
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//-------------------------------------------------------------------

// *************************************************************************************
// NOTE: THIS FILE IS PARSED BY SQLDRIVER - PLEASE DO THE FOLLOWING:
// *************************************************************************************
//
// 1. DO NOT put an error (or description) on more than one line
// 2. DO NOT include " in the description - this screws things up 
// 3. Make sure the comment is at the end of a line and has a space 
//    before the text ("// ")
// 4. All Errors should document error parameters after the error string in this format [#,#,#,s,s,s]
//		[,,,,,] == no error parameters for this error. 
//		[...] == we need to fix up use of this error - parameters are inconsitent
//		(nothing) == this error hasn't had its parameters recorded in this header file yet.
//                   It DOES NOT mean there are no parameters!
// 5. Our error number range is limited to the SQL Server error number range, Which is 25000 to 30000. 
//    DO NOT ADD ANY ERRORS ABOVE OR BELOW THIS RANGE!
// 6. If you add errors to this file, you must also add the error and the string to 
//    SSCE\SSCEError\SSCEError20xx.rc. The xx denotes the language for localization. (en = US English)
// *************************************************************************************

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// 								M I N O R   C O D E S
//
//	These are codes paired with a major code of E_FAIL for OLE/DB exposure.
//
//	Range (25000-30000)
//
// ***********************************************************************
// NOTE : Our error number range is limited to the SQL Server error number 
//		  range, Which is 25000 to 30000. DO NOT ADD ANY ERRORS ABOVE OR
//		  BELOW THIS RANGE!
// ***********************************************************************
//
//  SE Minor Range: 			25000 -- 25499
//
//	QP Minor Range: 			25500 -- 26999
//		25500 -- 25699	:	compile errors
//		25700 -- 25899	:	execute errors
//		25900 -- 26099	:	expr errors
//		26100 -- 26299	:	storage QP errors
//		26300 -- 26499	:	core QP errors
//		26500 -- 26999	:	reserved
//
//  OLEDB Layer					27000 -- 27499
//
//  SSCECommon Minor Range:		27500 -- 27749
//
//  Managed Code Minor Range:	27750 -- 27999
//
//  Connectivity Minor Range		28000 -- 30499
//		28000 -- 28499	:	Transport			
//		28500 -- 28999	:	Client Agent		
//		29000 -- 29499	:	Server Agent		
//		29500 -- 29999	:	Message Protocol
//		30000 -- 30499  :   SSCERelay
//		30500 -- 39499	:   Unused
//		39500 -- 39999	:   Miscellaneous (setup, etc.)	
//
//
// -------------------------------------------------------------------------------------
//
// Storage Engine	25000 - 25499
//
#define SSCE_M_NOCURRENTRECORD					25001			// Either the cursor is not on a row or there are no rows left. [,,,,,]
#define SSCE_M_INVALIDBUFFERSIZE				25002			// The specified buffer size is not valid. [Buffer size specified,,,,,]
#define SSCE_M_COLUMNLENGTHTOOBIG				25003			// The specified length is too long for the column type. [Specified length,Column data type,,,,]
#define SSCE_M_COLUMNNOTUPDATEABLE				25004			// The column cannot be modified. [,,,Column name,,]
#define SSCE_M_NULLINVALID						25005			// The column cannot contain null values. [,,,,,]
#define SSCE_M_INVALIDOPERATION					25006			// The operation cannot be performed on the object. [,,,,,]
#define SSCE_M_INTRANSACTION					25007			// The operation cannot be performed in a transaction. [,,,,,]
#define SSCE_M_INVALIDPARAMETER					25008			// The parameter is not valid. [Numeric parameter,,,String parameter,,]
#define SSCE_M_INVALIDPATH						25009			// The path is not valid. Check the directory for the database. [,,,Path,,]
#define SSCE_M_INVALIDFILENAME					25010			// The file name is not valid. Check the file name for the database. [,,,File name,,]
#define SSCE_M_INVALIDDATABASE					25011			// The file is not a valid database file. An internal error has occurred.  [,,,Databasename,,]
#define SSCE_M_UPDATENOTPREPARED				25012			// An internal error has occurred. [,,,,,]
#define SSCE_M_TABLENOTLOCKED					25013			// The operation is supported only on tables with exclusive locks. [,,,Table name,,]
#define SSCE_M_SYSCOLACCESSDENIED				25014			// Access to the system column is denied.  [,,,,,]
#define SSCE_M_CANTALLOCATEPAGE					25015			// A page cannot be allocated. There might not be enough memory on the device. [,,,,,]
#define SSCE_M_KEYDUPLICATE						25016			// A duplicate value cannot be inserted into a unique index. [,,,,,]
#define SSCE_M_DATABASECORRUPTED				25017			// The database file is corrupted. [,,,Database name,,]
#define SSCE_M_WRITECONFLICT					25018			// Failed to obtain a lock for a write operation. [,,,,,]
#define SSCE_M_READCONFLICT						25019			// Failed to obtain a lock for a read operation. [,,,,,]
#define SSCE_M_COMMITCONFLICT					25020			// Failed to obtain a lock for a commit. [,,,,,]
#define SSCE_M_SESSIONWRITECONFLICT				25021			// Failed to obtain lock for a read or write operation [,,,,,]
#define SSCE_M_NULLKEYDISALLOWED				25022			// Null keys are not allowed on indexes. [,,,,,]
#define SSCE_M_UNKNOWNVERSION					25023			// DEAD:Unknown object version [,,,TableName,,]
#define SSCE_M_CONNECTIONCONTROL				25024			// DEAD:Passive connection control prevented database open [,,,,,]
#define SSCE_M_INTEGRITYVIOLATION_MSTR			25025			// The primary key value cannot be deleted because references to this key still exist. [,,,Foreign key constraint name,,]
#define SSCE_M_INTEGRITYVIOLATION_SLV			25026			// A foreign key value cannot be inserted because a corresponding primary key value does not exist. [,,,Primary key constraint name,,]
#define SSCE_M_RECORDTOOBIG						25027			// The row being created exceeds the maximum size allowed. [,,,,,]
#define SSCE_M_INVALIDPASSWORD					25028			// The specified password does not match the database password. [,,,,,]
#define SSCE_M_RI_UPDATETWICE					25029			// An attempt was made to update a field in a record twice. [,,,,,]
#define SSCE_M_RI_DUPLICATEKEY					25030			// A duplicate key value was found as a result of referential integrity action. [,,,ForeignTableName,,]
#define SSCE_M_RI_KEYNULLDISALLOWED				25031			// A referential cascade action is setting a nonnull column to NULL. [,,,Foreign table name,,]
#define SSCE_M_RI_SESSIONWRITECONFLICT			25032			// A write conflict occurred because a referential integrity action is not allowed.  [,,,Foreign table name,,]
#define SSCE_M_ILLEGALCOLLATINGSEQ				25033			// The sort sequence is not valid. [,,,,,]
#define SSCE_M_FILELOCKVIOLATION				25034			// There is a file locking violation. [,,,,,]
#define SSCE_M_FILESHAREVIOLATION				25035			// There is a file sharing violation. A different process might be using the file. [,,,,,]
#define SSCE_M_FILELOCKINGUNAVAILABLE			25036			// File locking is not available. [,,,,,]
#define SSCE_M_DISKFULL							25037			// There is not enough disk space left for the database. [,,,,,]
#define SSCE_M_TEMPDISKFULL						25038			// There is not enough disk space left for the temp database. [,,,,,]
#define SSCE_M_FILEACCESSDENIED					25039			// Access to the database file is not allowed. [,,,File name,,]
#define SSCE_M_INVALIDAPPSIGNATURE				25040			// DEAD:Invalid application signature [,,,,,]
#define SSCE_M_INVALIDSORTDLL					25042			// The language required for this database file is not fully installed on this operating system. [,,,,,]
#define SSCE_M_TOOMANYACTIVEUSERS				25043			// DEAD:Too many active users [,,,,,]
#define SSCE_M_TOOMANYOPENFILES					25044			// Too many files are open [,,,,,]
#define SSCE_M_OBSOLETEDATABASEFORMAT			25045			// The database file format is obsolete. [,,,,,]
#define SSCE_M_FILENOTFOUND						25046			// The database file cannot be found. Check the path to the database. [,,,File name,,]
#define SSCE_M_READVERIFYERROR					25047			// Error occurred when reading from the database file. [,,,,,]
#define SSCE_M_NOMOREFILEHANDLES				25048			// No more file handles can be created. [,,,,,]
#define SSCE_M_DISKNOTREADY						25049			// The disk is not ready.  [,,,,,]
#define SSCE_M_SHARINGBUFFEREXCEEDED			25050			// Too many files are open for sharing.  [,,,,,]
#define SSCE_M_DISKIO							25051			// An internal error occurred. Unable to successfully execute disk IO on the file system. [,,,,,]
#define SSCE_M_THREADSNOTSUPPORTED				25052			// The operating system does not support threads. [,,,,,]
#define SSCE_M_KEYNOTMADE						25053			// DEAD:No call to MakeKey [,,,,,]
#define SSCE_M_TOOMANYKEYS						25054			// Too many columns were specified for the index. [,,,,,]
#define SSCE_M_CONTAINERNOTEMPTY				25055			// DEAD:Illegal attempt to delete a container that still had items [,,,,,]
#define SSCE_M_COLUMNWIDTHNOTEVEN				25056			// The size of the Unicode text column is not even. [Column size,,,Column name,,]
#define SSCE_M_LV_INVALIDAPPEND					25057			// DEAD:Cannot append long value [,,,,,]
#define SSCE_M_NOCURRENTINDEX					25058			// DEAD:Invalid operation with out a current index [,,,,,]
#define SSCE_M_ILLEGALINDEXCOLUMN				25059			// The column data type cannot be indexed. [,,,,,]
#define SSCE_M_REFERENCENOTFOUND				25060			// The reference does not exist. [,,,,,]
#define SSCE_M_INTERNAL_INVALIDDBID				25061			// DEAD:Internal error [,,,,,]
#define SSCE_M_OBJECTNOTREPLICABLE				25062			// The object is not replicable. [,,,Object name,,]
#define SSCE_M_ONLYONESPECIALCOLUMN				25064			// Only one ROWGUID, autoincrement and version are allowed for each column in a table. [,,,Object name,,]
#define SSCE_M_TOOMANYINDEXES					25065			// Too many indexes were created for the table. [,,,Table name,,]
#define SSCE_M_TOOMANYCOLUMNS					25066			// Too many columns were specified for the table. [Column count,,,Column name,,]
#define SSCE_M_TRANSTOODEEP						25067			// The transactions are nested too deeply. [,,,,,]
#define SSCE_M_SELFREFERENCE					25068			// A column in an index cannot reference itself. [,,,,,]
#define SSCE_M_NOPRIMARYINDEX					25069			// The referenced table must have a primary index. [,,,,,]
#define SSCE_M_DIFFNUMRELCOLUMNS				25070			// The definition of referring columns (such as number of columns or data types) in referential relationships must match the referred columns. [,,,,,]
#define SSCE_M_CONFLICTINGREFERENCE				25071			// A reference to this column already exists. [,,,Index name,,]
#define SSCE_M_PRIMARYKEYNULLCONFLICT			25072			// Primary keys cannot be created on columns that support null values. [,,,,,]
#define SSCE_M_TABLEMUSTHAVECOLUMNS				25073			// Tables must contain at least one column. [,,,,,]
#define SSCE_M_DEFAULTEXPRTOOLONG				25074			// The expression string for the default value cannot be longer than 255 characters. [Length of expression string,,,,,]
#define SSCE_M_DISALLOWIDENTITYNULL				25075			// Nullable columns cannot be identity columns.  [,,,,,]
#define SSCE_M_CONSTRAINTINUSE					25077			// The constraint cannot be removed because it is referenced by another constraint. [,,,Constraint name,,]
#define SSCE_M_INDEXINUSE						25078			// The index cannot be removed because it is being used to enforce a constraint. [,,,Index name,,]
#define SSCE_M_ENCRYPTEDDBMUSTHAVEPWD			25079			// A password must be specified when a database is created using encryption. [,,,,,]
#define SSCE_M_NOOSENCRYPTIONSUPPORT			25080			// The operating system does not support encryption. [,,,,,]
#define SSCE_M_VIOLATEDAUTOINCRANGE				25081			// The new value generated for the inserted row does not fall within the allowable range. [Generated value,,,,,]
#define SSCE_M_UNIQUENULLCONFLICT				25082			// Unique indexes and constraints cannot be created on columns that allow null values. [,,,,,]
#define SSCE_M_CYCLEDETECTED					25083			// The referential relationship will result in a cyclical reference that is not allowed. [,,,,,]
#define SSCE_M_RELCOLUMNSTYPEMISMATCH			25084			// The data types of the columns in the relationship do not match. [,,,,,]
#define SSCE_M_PKDUPLICATE						25085			// The table already has a primary key. [,,,,,]
#define SSCE_M_INVALIDINCREMENT					25086			// The identity column contains an increment value that is not valid. [,,,Column name,,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//
#define SSCE_M_GENERATEGUIDFAILED				25087			// A new GUID cannot be generated for the new row. [,,,,,]

//
// Errors defined for v2.0
//
#define SSCE_M_KEYTOOLARGE						25088			// The key size exceeded the maximum allowed. [,,,,,]
#define SSCE_M_LOCKCONFLICT						25089			// The lock failed due to conflict. [,,,,,]
#define SSCE_M_LOCKTIMEOUT						25090			// The system timed out waiting for a lock. [,,,,,]
#define SSCE_M_OUTOFLOCKS						25091			// The lock manager has no more space for locks. [,,,,,]
#define SSCE_M_INVALIDTEMPPATH					25092			// The path specified for the temp database is not valid. [,,,,,]

// Warnings
#define SSCE_WRN_COLUMNNULL						25200			// The column cannot be NULL. [,,,,,]
#define SSCE_WRN_BUFFERTRUNCATED				25201			// The data was truncated. [,,,,,]
#define SSCE_WRN_SEEKNOTEQUAL					25202			// Seek was not able to find an exact match. [,,,,,]
#define SSCE_WRN_OPTIONSIGNORED					25203			// Some specified options were ignored for this operation. [,,,,,]
#define SSCE_WRN_COLUMNDATATRUNCATED			25204			// The data was truncated. [,,,,,]
#define SSCE_WRN_DATABASEENCRYPTED				25205			// The database is encrypted. A password is necessary to access the database. [,,,,,]
#define SSCE_WRN_RI_RECORDSUPDATED				25206			// Additional records were updated as a result of referential integrity actions. [,,,,,]
#define SSCE_WRN_RI_RECORDSDELETED				25207			// Additional records were deleted as a result of referential integrity actions. [,,,,,]
#define	SSCE_WRN_ROWLOCKCONNECTFAILED			25208			// The request for a row-level lock was denied. [,,,,,]
#define	SSCE_WRN_PAGELOCKCONNECTFAILED			25209			// The request for a page-level lock was denied. [,,,,,]
//
//Warnings after this comment were added for v1.1 (PEBBLE) 
//


// --------------------------------------------------------------------------------------
//
// OLEDB Layer	27000 - 27499
//
#define SSCE_M_TOOMANYSESSIONSOUTSTANDING		27000			// Too many sessions are open. [,,,,,]

// Rich error messages for Set Property
//
#define SSCE_M_PRIMARYKEYPROPERTYNOTSETTABLE	27001			// The new column cannot be set as the PRIMARY KEY constraint. Create a table-level constraint instead. [,,,Table name,Column name,]
#define SSCE_M_UNIQUEKEYPROPERTYNOTSETTABLE		27002			// The new column cannot be set as a UNIQUE constraint. Create a table-level constraint instead.  [,,,Table name,Column name,]
#define SSCE_M_COLUMNNULLABILITYNOTALTERABLE	27003			// Column nullability cannot be changed after the column is created. [,,,Table name,Column name,]
#define SSCE_M_CANTMODIFYIDENTITYATTRIBUTE		27004			// The column cannot become an identity column after it is created. [,,,Table name,Column name,]
#define SSCE_M_INVALIDCOLUMNPROPERTY			27005			// The column property is not valid. [,,,Table name,Column name,Property]
#define SSCE_M_INVALIDSSCEDBINITPROPERTY		27006			// An attempt to set the SQLServer CE DBINIT property is not valid. [,,,,,]
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//


// --------------------------------------------------------------------------------------
//
// ssceCommon	27500 - 27749
//
#define SSCE_M_DECIMALOVERFLOW					27500			// An overflow occurred while setting decimal data. [,,,,,]
#define SSCE_M_DECIMALTRUNCATED					27501			// Decimal data was truncated. [,,,,,]
#define SSCE_M_CMN_INVALIDPARAMETER				27502			// The parameter is not valid. [,,,,,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//

// Warning
#define SSCE_WRN_DECIMALTRUNCATED				27700			// Decimal data was truncated. [,,,,,]
//
//Warnings after this comment were added for v1.1 (PEBBLE) 
//

// --------------------------------------------------------------------------------------
//
// Managed Code 	27750 - 27999
// 
// This range was added for Daytona
//
#define SSCE_M_SSCEMWFAILURE					27750			// Cannot load sscemw20.dll or ssceca20.dll is missing or not registered.  [,,,,,]




// --------------------------------------------------------------------------------------
//
// Replication Transport	28000 - 28499
//
#define SSCE_M_OPENTWICE						28000			// An internal error has occurred. [,,,,,]
#define SSCE_M_INTERNETREADERROR				28001			// A read operation from the Internet returned an incorrect number of bytes. [,,,,,]
#define SSCE_M_INTERNETWRITEERROR				28002			// A write operation to the Internet wrote an incorrect number of bytes. [,,,,,]
#define SSCE_M_INVALIDRSCBID					28003			// Either the computer running IIS is out of memory or an incorrect RSCBID was sent in a request. [RSCBID,,,,,]
#define SSCE_M_TOOMANYPENDINGREQUESTS			28004			// A SQL Server CE Server Agent queue is full and can take no further requests. Try again later. [,,,,,]
#define SSCE_M_COULDNTSTARTRECTHREAD			28005			// A thread to host the SQL Server Reconciler cannot be started. Check system resources. [,,,,,]
#define SSCE_M_CANTLOADRECRESOURCEDLL			28006			// The SQL Server Reconciler resource DLL cannot be loaded. [,,,,,]
#define SSCE_M_INVALIDRECONCILERPARAM			28007			// An internal error has occurred. [,,,,,]
#define SSCE_M_INVALIDTRNSPTCOMMAND				28008			// The transport failed due to incorrect message format or content. [Command number,,,,,]
#define SSCE_M_CANTCOCREATERECONCILER			28009			// An instance of the SQL Server Reconciler cannot be created. [,,,,,]
#define SSCE_M_CANTCOCREATERECONERROR			28010			// An instance of the SQL Server Reconciler error object cannot be created. [,,,,,]
#define SSCE_M_HTTPSTATUSDENIED					28011			// Authentication failed on the computer running IIS.  [,,,,,]
#define SSCE_M_HTTPSTATUSPROXYAUTHREQ			28012			// Authentication failed on the proxy server. [,,,,,]
#define SSCE_M_HTTPSTATUSBADREQUEST				28013			// The HTTP request failed due to incorrect format or content. [,,,,,]
#define SSCE_M_HTTPSTATUSPAYMENTREQ				28014			// This site requires payment before it can be accessed. [,,,,,]
#define SSCE_M_HTTPSTATUSFORBIDDEN				28015			// A Secure Sockets Layer (SSL) connection is required to access this site. [,,,,,]
#define SSCE_M_HTTPSTATUSNOTFOUND				28016			// The URL cannot be found.   [,,,,,]
#define SSCE_M_HTTPSTATUSBADMETHOD				28017			// An incorrect or unsupported HTTP function call was made. [,,,,,]
#define SSCE_M_HTTPSTATUSNONEACCEPT				28018			// An HTTP function failed to establish a connection to the computer running IIS.  [,,,,,]
#define SSCE_M_HTTPSTATUSREQTIMEOUT				28019			// The request timed out on the computer running IIS. [,,,,,]
#define SSCE_M_HTTPSTATUSCONFLICT				28020			// The computer running IIS returned a Conflict status. [,,,,,]
#define SSCE_M_HTTPSTATUSGONE					28021			// The computer running IIS returned a Gone status. [,,,,,]
#define SSCE_M_HTTPSTATUSSERVERERROR			28022			// An error has occurred on the computer running IIS. [,,,,,]
#define SSCE_M_HTTPSTATUSNOTSUPPORTED			28023			// The computer running IIS returned a Not Supported status. [,,,,,]
#define SSCE_M_HTTPSTATUSBADGATEWAY				28024			// A gateway error has occurred. [,,,,,]
#define SSCE_M_HTTPSTATUSSVCUNAVAIL				28025			// The IIS service is not available. [,,,,,]
#define SSCE_M_HTTPSTATUSGATTIMEOUT				28026			// The gateway has timed out. [,,,,,]
#define SSCE_M_HTTPSNOTSUPPORTED				28027			// HTTPS is not supported on Windows CE 1.1-based devices.  [,,,,,]
#define SSCE_M_REPLPROHIBITED					28028			// Replication is prohibited for this SQL Server CE Server Agent on the computer running IIS. [,,,,,]
#define SSCE_M_RDAPROHIBITED					28029			// RDA is prohibited for this SQL Server CE Server Agent on the computer running IIS. [,,,,,]
#define SSCE_M_COMPRESSIONFAILED				28030			// Internal error: Compression failed. [,,,,,]
#define SSCE_M_DECOMPRESSIONFAILED				28031			// Internal error: Decompression failed. [,,,,,]
#define SSCE_M_FAILUREINITIALIZINGINTERNET		28032			// Internet Open failed at the client. [,,,,,]
#define SSCE_M_INVALIDURLSYNTAX					28033			// The URL syntax is not valid. [,,,URL,,]
#define SSCE_M_INTERNETCONNECTFAILURE			28034			// An Internet connection cannot be established. [,,,,,]
#define SSCE_M_FAILUREQUERYINGHEADER			28035			// Header information is either corrupted or missing. [,,,Header name,,]
#define SSCE_M_CLOSEINTERNETHANDLEFAILED		28036			// An internal or network problem was encountered while trying to close an Internet handle. [,,,,,]
#define SSCE_M_HTTPSENDREQUESTFAILED			28037			// A request to send data to the computer running IIS has failed. For more information, see HRESULT. [,,,,,]
#define SSCE_M_MAXIMUMRSCBSEXCEEDED				28038			// The connection was refused by the computer running IIS because the maximum number of RSCBs has been exceeded. Try connecting later. [,,,,,]
#define SSCE_M_REQUESTSUPERSEDED				28039			// The request was superseded by a subsequent request. [,,,,,]
#define SSCE_M_RESENTCOMMANDIGNORED				28040			// The command has been completed. The resent command was ignored. [,,,,,]
#define SSCE_M_RESENDREQUEST					28041			// An internal error occurred [,,,,,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//


// --------------------------------------------------------------------------------------
//
// Client Agent		28500 - 28999
//
#define SSCE_M_CANTCOCREATESSCEERRORS				28500		// An instance of the SSCEErrors collection object cannot be created. [,,,,,]
#define SSCE_M_SUBSCRIBERDATABASEPATHMISSING		28501		// The SubscriberConnectionString property is not specified. [,,,,,]
#define	SSCE_M_INTERNETURLPROPERTYMISSING			28502		// The InternetURL property is not specified. [,,,,,]
#define SSCE_M_PUBLISHERPROPERTYMISSING				28503		// The Publisher property is not specified. [,,,,,]
#define	SSCE_M_PUBLISHERDATABASEPROPERTYMISSING		28504		// The PublisherDatabase property is not specified. [,,,,,]
#define SSCE_M_PUBLICATIONPROPERTYMISSING			28505		// The Publcation property is not specified. [,,,,,]
#define SSCE_M_SUBSCRIBERPROPERTYMISSING			28506		// The Subscriber property is not specified. [,,,,,]
#define SSCE_M_MISSINGDISTRIBUTORPROPERTIES			28507		// Some Distributor properties are not specified.  [,,,,,]
#define SSCE_M_MISSINGDISTRIBUTORADDRESS			28508		// The DistributorAddress property is not specified. [,,,,,]
#define SSCE_M_MISSINGPUBLISHERADDRESS				28509		// The PublisherAddress property is not specified. [,,,,,]
#define SSCE_M_INVALIDPUBLISHERSECURITYMODE			28510		// The value specified for the PublisherSecurityMode property is not valid [Publisher security mode,,,,,]
#define SSCE_M_INVALIDDISTRIBUTORSECURITYMODE		28511		// The value specified for the DistributorSecurityMode property is not valid. [Distributor security mode,,,,,]
#define SSCE_M_INVALIDQUERYTIMEOUTVALUE				28512		// The value specified for the QueryTimeout property is not valid. [Query timeout value,,,,,]
#define SSCE_M_INVALIDLOGINTIMEOUTVALUE				28513		// The value specified for the LoginTimeout property is not valid. [Login timeout value,,,,,]
#define SSCE_M_INVALIDDISTRIBUTORNETWORK			28514		// The value specified for the DistributorNetwork property is not valid. [Distributor network,,,,,]
#define SSCE_M_INVALIDEXCHANGETYPE					28515		// The value specified for the ExchangeType property is not valid. [Exchange type,,,,,]
#define SSCE_M_INVALIDVALIDATIONTYPE				28516		// The value specified for the ValidateType property is not valid. [Validation type,,,,,]
#define SSCE_M_INITIALIZEFAILED						28517		// The Initialize method failed. [,,,,,]
#define SSCE_M_ADDSUBSCRIPTIONFAILED				28518		// The AddSubscription method failed. [,,,,,]
#define SSCE_M_UNSUPPORTEDDBDROPOPTION				28519		// The value specified for DBDROPOPTION is not valid. [,,,,,]
#define SSCE_M_DROPSUBSCRIPTIONFAILED				28520		// The DropSubscription method failed [,,,,,]
#define SSCE_M_DBALREADYREPLICABLE					28521		// The SQL Server CE database is already enabled for replication. [,,,,,]
#define SSCE_M_RUNFAILED							28522		// The Run method failed. [,,,,,]
#define SSCE_M_INITIALIZATIONFAILED					28523		// Internal error: Initializing an internal object failed. [,,,,,]
#define SSCE_M_TERMINATEFAILED						28524		// The Terminate method failed.  [,,,,,]
#define SSCE_M_REINITIALIZESUBSCRIPTIONFAILED		28525		// The ReinitializeSubscription method failed.  [,,,,,]
#define SSCE_M_UNEXPECTEDROCENCOUNTERED				28526		// The message contains an unexpected replication operation code. [ROC,,,,,]
#define SSCE_M_COLUMNORDINALNOTFOUND				28527		// Internal error: The column ordinal cannot be found. [Column ordinal,,,Table name,,]
#define SSCE_M_NICKNAMEGENERATIONFAILED				28528		// A nickname was not generated. [,,,,,]
#define SSCE_M_DBNOTREPLICABLE						28529		// The SQL Server CE database is not enabled for replication. [,,,,,]
#define SSCE_M_FAILUREREADINGREGISTRY				28530		// Reading the registry failed. [,,,,,]
#define SSCE_M_FAILUREWRITINGREGISTRY				28531		// Writing to the registry failed [,,,,,]
#define SSCE_M_TRANSPORTREADRETURNEDNODATA			28532		// A read operation from the transport returned no data. [,,,,,]
#define SSCE_M_CANTCOCREATESQLSERVERCE				28533		// Another instance of SQL Server CE cannot be created. [,,,,,]
#define SSCE_M_RDAPULLFAILED						28534		// The Pull method failed. [,,,,,]
#define SSCE_M_RDAPUSHFAILED						28535		// The Push method failed. [,,,,,]
#define SSCE_M_RDASUBMITSQLFAILED					28536		// The SubmitSQL method failed. [,,,,,]
#define SSCE_M_IDENTITYRANGE						28537		// The identity range was not established. [,,,,,]
#define SSCE_M_INVALIDARTICLE						28538		// An article in the message is not valid. [Article nickname,,,,,]
#define SSCE_M_OBJECTALREADYINITIALIZED				28539		// The object is already initialized [,,,Connection string,,]
#define SSCE_M_OBJECTNOTINITIALIZED					28540		// The object is not initialized [,,,Connection string,,]
#define SSCE_M_LOCALTABLENAMEMISSING				28541		// The value for the LocalTableName parameter is not specified for the Push call. [,,,,,]
#define SSCE_M_NOTALLPARAMSSETFORPULL				28542		// Some parameters were not specified for the Pull method. [,,,,,]
#define SSCE_M_TABLENOTTRACKED						28543		// The table is not a tracked table.  [,,,Table name,,]
#define SSCE_M_RDAERRORROWSRETURNED					28544		// The Push method returned one or more error rows. See the specified error table. [,,,Error table name,,]
#define SSCE_M_SUBSCRIBERCONNECTIONSTRINGMISSING	28545		// The OLE DB connection string is not valid. [,,,,,]
#define SSCE_M_INVALIDCONNECTIONSTRING				28546		// The specified OLE DB for SQL Server CE connection string is not valid. [,,,Connection string,,]
#define SSCE_M_DATABASEALREADYOPEN					28547		// The SQL Server CE database cannot be compacted because it is open. [,,,,,]
#define SSCE_M_PARAMETERTOOLARGE					28548		// A specified parameter is too large. [Size of parameter,Max size,,Parameter value,,]
#define SSCE_M_FAILEDRETRIES						28549		// The row update or insert cannot be reapplied due to an integrity violation. [,,,,,]
#define SSCE_M_COMPACTFAILED						28550		// The SQL Server CE database cannot be compacted. [,,,,,]
#define SSCE_M_MISSINGCONNECTSTRING					28551		// The LocalConnectionString parameter is not specified for either a Push or Pull method. [,,,,,]
#define SSCE_M_ROW_RUID								28552		// This table does not have a ROWGUID column. [,,,,,]
#define SSCE_M_SPECIAL_COLUMN						28553		// This table does not have a ROWGUID column nor an Identity range. [,,,,,]
#define SSCE_M_VALIDATEFAILED						28554		// The number of rows at the client does not match the number of rows at the server. [SQL Server count, SQL Server CE count,,,,]
#define SSCE_M_FAILEDMAKEDBREP						28555		// The SQL Server CE database cannot be replicated. The subscription must be reinitialized. [,,,,,]
#define SSCE_M_TABLENAMEMISSING						28556		// The LocalTableName parameter is not specified. [,,,,,]
#define SSCE_M_UNUSABLEDATABASE						28557		// The SQL Server CE database cannot be used for replication. Delete and then re-create the database. [,,,Connection string,,]
#define SSCE_M_CREATEDATABASE						28558		// SQL Server CE encountered problems in creating the SQL Server CE database. [,,,Connection string,,]
#define SSCE_M_OPENDATABASE							28559		// SQL Server CE encountered problems in opening the SQL Server CE database. [,,,Database name,,]
#define SSCE_M_EXECUTEFAILED						28560		// The OLE DB Execute method failed. The SQL statement is not valid. [,,,SQL statement,,]
#define SSCE_M_ROLLBACKFAILED						28561		// Internal error: Failed to roll back changes. [,,,,,]
#define SSCE_M_PROCESSDELETEFAILED					28562		// The delete message for the server was not created. [,,,,,]
#define SSCE_M_CLIENTAGENT_NOMEMORY					28563		// The device does not have sufficient memory. [,,,,,]
#define SSCE_M_DELETEFAILED							28564		// The delete action failed. [,,,Table name,,]
#define SSCE_M_INSERTFAILED							28565		// The insert action failed. [,,,Table name,,]
#define SSCE_M_UPDATEFAILED							28566		// The update action failed. [,,,Table name,,]
#define SSCE_M_OPENTABLEFAILED						28567		// The table cannot be opened. [,,,Table name,,]
#define SSCE_M_CLOSETABLEFAILED						28568		// The table cannot be closed. [,,,Table name,,]
#define SSCE_M_INVALIDINDEX							28569		// The index cannot be set on the ROWGUID column. [,,,,,]
#define SSCE_M_ERRORFETCHINGDATA					28570		// The data row cannot be accessed. [,,,,,]
#define SSCE_M_ERRORFETCHINGCOLUMNDATA				28571		// The column data for a row cannot be accessed. [,,,,,]
#define SSCE_M_ERRORSETTINGDATA						28572		// Row data cannot be set. [,,,,,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//
#define SSCE_M_TABLEALREADYEXISTS					28573		// The LocalTableName parameter is already specified. [,,,Table name,,]

// Errors after this comment were added for v2.0 (DAYTONA)
//
#define SSCE_M_INTERNALERROR						28574		// An internal error occurred. [ID,,,,,]
#define SSCE_M_INSUFFICIENTLINEAGESPACE				28575		// There is not enough space for lineage in a row. [,,,,,]
#define SSCE_M_ERRORBINDINGCOLUMN					28576		// DEAD:Couldn't bind to column [,,,columnname,,]
#define SSCE_M_SCHEMAHASCHANGED						28577		// The schema has changed since the table was pulled. [,,,,,]
#define SSCE_M_UNSUPPORTEDDATATYPE					28578		// The data type is not valid. [Datatype,,,,,]
#define SSCE_M_MOVENEXTFAILED						28579		// The move to the next row failed. [,,,,,]
#define SSCE_M_INVALIDCOMPACTPARAMETER				28580		// The specified parameter is not valid [,,,,,]
#define SSCE_M_FAILUREMAKINGMESSAGE					28581		// The message cannot be built. The Make Message failed. [,,,,,]
#define SSCE_M_INVALIDRDAPARAMETER					28582		// The parameter is either not specified or not valid. [,,,,,]
#define SSCE_M_PUBLISHERLOGINMISSING				28583		// The Publisher login property is not specified.  [,,,,,]



// Warnings
//
#define SSCE_WRN_RDAERRORROWSRETURNED				28800		// The Push returned one or more error rows. See the specified error table. [,,,,,]
#define SSCE_WRN_SUBWINSERRORSFIXED					28801		// A data error occurred and was fixed. Call the Run method again to merge data. [,,,,,]
//
//Warnings after this comment were added for v1.1 (PEBBLE) 
//

// --------------------------------------------------------------------------------------
//
// Server Agent		29000 - 29499
//
#define SSCE_M_UNEXPECTEDSTATEREADINGGENINFO		29000		// An unexpected state was found when reading generation information. [,,,,,]
#define SSCE_M_EXPECTEDINSERTSONLY					29001		// An unexpected state was encountered when processing inserts.  [,,,,,]
#define SSCE_M_IDENTITYSEEDORRANGETOOLARGE			29002		// The identity seed or range is too large.  [,,,,,]
#define SSCE_M_UNEXPECTEDSTATEPROCESSINGTOMBSTONES 	29003		// An unexpected state was encountered when processing deletes.  [,,,,,]
#define SSCE_M_CANTCOCREATEMSDAINITIALIZE			29004		// The OLE DB provider for SQL Server CE cannot be initialized.  [,,,,,]
#define SSCE_M_RSCBWITHSAMEEXCHANGEID				29005		// A replication session control block already exists. [,,,,,]
#define SSCE_M_SQLRECONCILERFAILED					29006		// A call to SQL Server Reconciler failed. [,,,,,]
#define SSCE_M_QUERYCANNOTBETRACKED					29007		// The query cannot be tracked. There might not be a primary key, or the query may involve multiple tables.  [,,,Query string,,]
#define SSCE_M_PREPAREUPDATECMDFAILED				29008		// The prepare/update command process failed. [,,,,,]
#define SSCE_M_EXPRESSIONWITHOUTALIAS				29009		// The expression or literal cannot be processed without an alias. [,,,,,]
#define SSCE_M_NOPRIMARYKEY							29010		// The table does not have a primary key.  [,,,Table name,,]
#define SSCE_M_ROWVERCOLUMN							29011		// The table contains a timestamp or another row versioning column. [,,,Table name,,]
#define SSCE_M_INVALIDPUSHSEQUENCE					29012		// An internal error occurred during the push operation in the SQL Server CE Server Agent. [,,,,,]
#define SSCE_M_PUSHPARAMSETONVALUE					29013		// The attempt to replace an existing parameter value in the push operation failed. [,,,,,]
#define SSCE_M_NOTALLPARAMSETFORPUSH				29014		// Incomplete parameters were set for the push operation. [,,,,,]
#define SSCE_M_PUSHPKCOLCOUNTMISMATCH				29015		// The expected column count does not match the data column specified. [Supplied count, Expected count,,,,]
#define SSCE_M_VF_MISSINGSERVERCOLUMN               29016       // Client-side and server-side columns for the RDA table do not match. [,,,Table name,,]
#define SSCE_M_VF_MISMATCHEDTYPES                   29017       // The OLE DB data type information in the SQL Server CE columns does not match the information in the SQL Server columns for the RDA table. [Client type, Server type,,Table name,,]
#define SSCE_M_VF_MISSINGTABLE                      29018       // No columns were found. Either the table name is missing or the permissions are not valid. [,,,,,]
#define SSCE_M_NOROWSAFFECTED                       29020       // A row cannot be updated or deleted in the SQL Server using RDA. [,,,,,]
#define SSCE_M_TOOMANYROWSAFFECTED                  29021       // More than one row was affected by an update or delete operation in the SQL Server using RDA. [,,,,,]
#define SSCE_M_INCORRECTPROVIDERVERSION				29022		// The version of the Microsoft OLE DB Provider for SQL Server is not correct. [,,,Version,,]
#define SSCE_M_INCORRECTPROVIDER					29023		// The OLE DB Provider for SQL Server on the computer running IIS is not compatible with SQL Server CE. [,,,Provider,,]
#define SSCE_M_VF_PRIMARYKEYCHANGE                  29024       // The push operation cannot match the orders of the client-side and server-side primary key columns. [,,,,,]
#define SSCE_M_VF_MISSINGSERVERPKCOLUMN             29025       // The push operation cannot find a primary key column on the SQL Server table. [,,,,,]
#define SSCE_M_VF_EXTRASERVERPKCOLUMN               29026       // The push operation has found an extra primary key column on the SQL Server table. [,,,,,]
#define SSCE_M_MISSINGCOLUMNSCHEMACOLS              29027       // The current OLE DB provider is not supported. [,,,,,]
#define SSCE_M_UNSUPPORTEDOLEDBTYPE                 29028       // The OLE DB data type is not supported by the Microsoft OLE DB Provider for SQL Server. [Data type,,,,,]
#define SSCE_M_VF_NONLOBIUNKNOWN                    29029       // The SQL Server CE and SQL Server column meta data information does not match [,,,,,]
#define SSCE_M_NOTRACKPKCOLMISSING					29030		// The query cannot be tracked because one or more columns of the primary key are missing. [,,,,,]
#define SSCE_M_NOTRACKCOMPUTEDCOLUMN				29031		// The query cannot be tracked because the table includes a computed column that cannot be pulled. [,,,,,]
#define SSCE_M_NOTRACKMULTITABLE					29032		// The query cannot be tracked because it is a multi-table query. [,,,,,]
#define SSCE_M_INVALIDCODEPAGE						29033		// The server code page, or the override code page, is either not valid or not installed on the computer running IIS. [Code page,,,,,]
//***** Don't add any more errors here, read the comments below and add appropriately
// 
// Following error is a V1.1 (Pebble) error
// included here due to the numbering problem that produced the gap below.
//
#define SSCE_M_MISMATCHEDCOMPONENTS					29034		// The SQL Server CE Client Agent and SQL Server CE Server Agent component versions do not match. [Client version,Server version,,SQL Server CE Client Agent version,SQL Server CE Server Agent version,]
// Note:  There is a gap in the numbering here.  Do not renumber the following, since they are already documented.
//		  New errors for this section should continue from the highest value in the section.
//		  The two errors below are from V1.0	
//
#define SSCE_M_RSCBTIMEDOUT							29042		// The replication session control block was inactive for longer than the cleanup interval. [,,,,,]
#define SSCE_M_NOPROXYSERVERSUPPORT					29043		// The operating system on the Windows CE-based device does not support authenticated proxy servers. [,,,,,]
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//
#define SSCE_M_FILERENAMEFAILED						29044		// SQL Server CE failed in renaming an .IN file to .OUT. [,,,,,]
#define SSCE_M_INITIALIZERECONCILERFAILED			29045		// Initializing SQL Server Reconciler has failed. [,,,,,]

//
// Errors after this comment were added for v2.0 (DAYTONA)
//
#define SSCE_M_READONLY								29046		// This cursor is read-only. [,,,,,]
#define SSCE_M_PROCESSINGROWS						29047		// Row processing resulted in an error. [,,,,,]
#define SSCE_M_IMPERSONATIONFAILED					29048		// The Windows user account could not be impersonated at the computer running IIS. [,,,,,]

// --------------------------------------------------------------------------------------
//
// Message Protocol		29500- 29999
//	
#define SSCE_M_TRANSPORTREADWRONGSIZE				29500		// The transport read operation returned an incorrect data length. [Returned length,Expected length,,,,]
#define SSCE_M_UNEXPECTEDSTATEWRITINGMESSAGE		29501		// Internal error: The write message failed. [,,,,,]
#define SSCE_M_INVALIDCOLUMNNUMBER					29502		// The column number is not valid. [Column number,,,Table name,,]
#define SSCE_M_CANTFINDCOLUMNBYNAME					29503		// The column cannot be found. [,,,Column name,Table name,]
#define SSCE_M_NAMESBUFFERTOOSMALL					29504		// Internal error: The names buffer is too small. [,,,,,]
#define SSCE_M_INCORRECTNUMBEROFCOLUMNSPROCESSED 	29505		// Incorrect number of columns processed [columns processed,expected number,,,,]
#define SSCE_M_UNEXPECTEDSTATEGETTINGCOLUMNS		29506		// Internal error: The get column failed. [,,,,,]
#define SSCE_M_UNEXPECTEDSTATEGETCOLLENGTH			29507		// Internal error: The get column length failed. [,,,,,]
#define SSCE_M_UNEXPECTEDSTATEGETCOLDATA			29508		// Internal error: The get column data failed. [,,,,,]
#define SSCE_M_UNEXPECTEDSTATESETCOLUMN				29509		// Internal error: The set column failed. [,,,,,]
#define SSCE_M_STREAMNOTEXHAUSTED					29510		// The ISequentialStream interface detects that the BLOB data stream is not empty. [,,,,,]
#define SSCE_M_GARBLEDMESSAGEFROMSERVER				29511		// The message received from the server cannot be decoded. [,,,,,]
#define SSCE_M_GARBLEDMESSAGEFROMCLIENT				29512		// The message received from the client cannot be decoded. [,,,,,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//


// --------------------------------------------------------------------------------------
//
// Query Compile		25500 -- 25699
//	
#define SSCE_M_QP_BADOPT	   						25500		// The execution plan cannot be generated. [,,,,,]
#define SSCE_M_QP_BADPARSE	   						25501		// There was an error parsing the query. [Token line number,Token line offset,,Token in error,,]
#define SSCE_M_QP_SETLISTCOUNT						25502		// The count of column names and source expressions do not match. [Column name count,Source expression count,,,,]
#define SSCE_M_QP_BAD_COLNAME						25503		// The column name is not valid. [,,,Node name (if any),Column name,]
#define SSCE_M_QP_INSERTMISSINGCOL					25504		// DEAD:Integrity violation, not an identity column or does not have the nullability property [,,,Name of Column in error,,]
#define SSCE_M_QP_BADCNSTS							25505		// The constraint specified is not valid. [,,,,,]
#define SSCE_M_QP_NOMEMORY							25506		// There is not enough memory to complete the operation. [,,,,,]
#define SSCE_M_QP_UNCLOSEDCOMMENT					25507		// The comment block is not complete. [,,,,,]
#define SSCE_M_QP_TOKTOOLONG						25508		// The token specified is too long. The maximum length is 128 characters. [Maximum size of token (if known),,,Token(if known),,]
#define SSCE_M_QP_ATOF_OVERFLOW						25509		// A conversion from string to float data failed. [,,,String,,]
#define SSCE_M_QP_NUME_OUTOFRANGE					25510		// A conversion from string to numeric data failed. [,,,String value,,]
#define SSCE_M_QP_UNCLOSEDQUOTE						25511		// A quotation mark delimiter is missing from the end of the query. [,,,Query string,,]
#define SSCE_M_QP_BADMONEY							25512		// Conversion from string to money data failed. [,,,Bad value,,]
#define SSCE_M_QP_IDENTWITHNULL						25513		// Null values are not valid identifiers. [,,,Identifier,,]
#define SSCE_M_QP_NAME_EMPTY						25514		// The identifier cannot be an empty string.  [,,,,,]
#define SSCE_M_QP_NO_SCALAR_IN_SELECT				25515		// In aggregate and grouping expressions, the SELECT clause can contain only aggregates and grouping expressions. [,,,Select clause,,]
#define SSCE_M_QP_NO_AGG_IN_AGG						25516		// Nested aggregate expressions are not allowed. [,,,Query string,,]
#define SSCE_M_QP_NO_SCALAR_IN_HAVING				25517		// In aggregate and grouping expressions, the HAVING clause can contain only aggregate functions and grouping expressions.  [,,,Having clause,,]
#define SSCE_M_QP_NO_SCALAR_IN_AGGRORDERBY			25518		// In aggregate and grouping expressions, the ORDER BY clause can contain only aggregate functions and grouping expressions. [,,,Order by clause,,]
#define SSCE_M_QP_NO_AGG_IN_GROUPBY					25519		// Expressions in the GROUP BY list cannot contain aggregate functions. [,,,Group by list,,]
#define SSCE_M_QP_NO_AGG_IN_ORDERBY	    			25520		// Expressions in the ORDER BY list cannot contain aggregate functions. [,,,Order by list,,]
#define SSCE_M_QP_NO_DUPEXPR_IN_ORDERBY				25521		// The ORDER BY list cannot have duplicate expressions. [,,,Order by list,Duplicate expression,]
#define SSCE_M_QP_DUP_TABLE_ALIAS					25522		// The table aliases must be unique. [,,,Name of duplicate alias,,]
#define SSCE_M_QP_DUP_COLUMN_ALIAS					25523		// The column aliases must be unique. [,,,Name of duplicate alias,,]
#define SSCE_M_QP_HAVING_WITHOUT_GROUPBY			25524		// A HAVING clause must have a GROUP BY clause (implicit or explicit). [,,,Having clause,,]
#define SSCE_M_QP_NO_AGG_IN_WHERE					25525		// The WHERE clause cannot refer to aggregate expressions. [,,,Where clause,,]
#define SSCE_M_QP_DUP_CNST							25526		// Duplicate constraints cannot be specified. [,,,Constraint,,]
#define SSCE_M_QP_AMBIGUOUS_FUNC					25527		// A function with the same signature already exists. [,,,Name of function,,]
#define SSCE_M_QP_CANT_COCREATE						25529		// The OLE DB provider cannot be instantiated. [,,,,,]
#define SSCE_M_QP_SETLIST_DUPCOLNAME				25530		// A column with the same name has already been specified. [,,,Name of duplicate column,,]
#define SSCE_M_QP_AMBIGUOUS_COLNAME					25531		// The column name cannot be resolved to a table. Specify the table to which the column belongs. [,,,Name of ambiguous column,,]
#define SSCE_M_QP_BAD_FLOAT_PRECISION				25532		// The float precision is not valid. [,,,,,]
#define SSCE_M_QP_LONGIDNAME						25533		// The identifier name is too long. [,,,Name of column(if known),,]
#define SSCE_M_QP_LOB_IN_ORDERBY					25534		// Large objects (ntext and image) cannot be used in ORDER BY clauses. [,,,Order by clause,,]
#define SSCE_M_QP_INVALIDMETA_FOR_LOB				25536		// The meta data for the large object is not valid. [,,,,,]
#define SSCE_M_QP_SUBQUERY_COLUMN_MISMATCH			25537		// The columns in the subquery do not match. [,,Column name,,]
#define SSCE_M_QP_ENCRYPTION_NO_DBPASSWORD			25538		// The database is encrypted. A password must be specified. [,,,,,]
#define SSCE_M_QP_BADLIKEESCCHAR					25539		// The escape character for the LIKE predicate is not valid [,,,Escape character,,]
#define SSCE_M_QP_BADDATEOPTION						25540		// The datepart is not valid. [,,,Portion of data in error,,]
#define SSCE_M_QP_INVALID_IDENTIFIER				25541		// The identifier is not valid. [,,,Identifier,,]
#define SSCE_M_QP_READONLYINSERT                	25542		// Data cannot be inserted into a read-only table. [,,,Name of read-only table,,]
#define SSCE_M_QP_READONLYUPDATE                	25543		// Data cannot be updated in a read-only table. [,,,Name of read-only table,,]
#define SSCE_M_QP_READONLYDELETE                	25544		// Data cannot be deleted from a read-only table. [,,,Name of read-only table,,]
#define SSCE_M_QP_NOINSERTCOLUMNS               	25545		// A value must be provided for the INSERT statement into <table_name>. [,,,Table name,,]
#define SSCE_M_QP_MODIFYPROTECTEDCOL            	25546		// The column is read-only and cannot be modified. [,,,Column name,,]
#define SSCE_M_QP_SCALARORDERBYNOTSUPPORTED     	25547		// ORDER BY <column number> not supported [,,,,,]
#define SSCE_M_QP_LOB_IN_DISTINCT               	25548		// DISTINCT cannot be used with  ntext and image data.  [,,,,,]
#define SSCE_M_QP_LOB_IN_GROUPBY					25549		// GROUP BY cannot be used with ntext and image data. [,,,,,]
#define SSCE_M_QP_MULTI_IDENTITY_COLUMNS			25550		// Too many identity columns are specified for the table. Only one identity column for each table is allowed. [,,,SQL statement,,]
#define SSCE_M_QP_BAD_IDENTITY_TYPE					25551		// The identity column must be an integer data type and cannot be NULL. [,,,,,]
#define SSCE_M_QP_GROUPBY_MUST_REFERENCE_COLS		25552		// Elements in the GROUP BY clause must reference columns in the select list.  [,,,SQL statement,,]
#define SSCE_M_QP_CASCADEONSELFREFFK                25553       // A foreign key constraint that has an UPDATE or a DELETE CASCADE rule and self-references a column in the same table, is not allowed. [,,,,,]
#define SSCE_M_QP_NODEFAULTONIDENTITYCOL            25554       // Defaults cannot be created on identity columns. [,,,Name of table,Name of column,,]
#define SSCE_M_QP_SUBQUERY_NOLOBCOLUMNS			    25555		// Subquery columns cannot be ntext or image data types. [,,,,,]
#define SSCE_M_QP_INVALID_CHARACTER_IN_IDENTIFIER   25556		// The number sign (#) is reserved and cannot be used as the first character of an identifier. [,,,Invalid identifier,,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
//Errors after this comment were added for v2.0 
//
#define SSCE_M_UNIONCOLUMNCOUNT						25557			// Expressions combined by a UNION operator must have the same number of columns. [,,,,,]
#define SSCE_M_DIFFERENTCOLUMNCOUNT					25558			// The number of columns in the query and the table must match. [,,,,,]

// --------------------------------------------------------------------------------------
//
// Query Expression Service		25900 -- 26099
//	
#define SSCE_M_QP_BADEXPR	   						25900		// The query processor encountered an error evaluating the expression. [,,,,,]
#define SSCE_M_QP_OVFLO		   						25901		// Expression evaluation caused an overflow. [,,,Expression,,]
#define SSCE_M_QP_EXPRTYPECHECK						25902		// The data types in the IN expression do not match.  [,,,Expression,,]
#define SSCE_M_QP_DATEPART							25903		// There is an error in the datepart format.  [,,,Expression,,]
#define SSCE_M_QP_DATEABORT							25904		// There is an error in the date format. [,,,Expression,,]
#define SSCE_M_QP_SYNTAX							25905		// A syntax error occurred [,,,Expression,,]
#define SSCE_M_QP_CONVSYNTAX						25906		// Conversion failed due to a syntax error [,,,Expression,,]
#define SSCE_M_QP_MONEYOVERFLOW						25907		// An overflow occurred while converting the string data type to money. [,,,String,,]
#define SSCE_M_QP_NOCOLUMNREFINDEFAULT				25909		// Default values cannot have column references. [,,,Expression,,]
#define SSCE_M_QP_NOAGGRINDEFAULT					25910		// Default values cannot have aggregate functions. [,,,Expression,,]
#define SSCE_M_QP_NOAGGRONLOB						25911		// Aggregate value expressions cannot have ntext or image data. [,,,Expression,,]
#define SSCE_M_QP_NOCONVDUETOTRUNC					25912		// The original data is truncated and cannot be converted. [,,,Expression,,]
#define SSCE_M_QP_ARITHOPNOTSUPPORTED				25913		// The arithmetic operator is not supported for the data type [,,,Expression,,]
#define SSCE_M_QP_NOMODULOSUPPORT					25914		// Modulo is not supported on real, float, money, and numeric data types. [,,,Data type,,]
#define SSCE_M_QP_SUBQUERYINVALIDOP             	25916		// An operator on the result data type from a subquery is not valid.  [,,,Operator,Subquery expression,]
#define SSCE_M_QP_NOBITOPSUPPORT                	25917		// Bit operators (&,|,^,~) are not supported on real, float, money, and numeric data types. [,,,Data type,,]
#define SSCE_M_QP_BADARITHTYPE                  	25918		// The data type is not valid for the arithmetic operation. [Data type,,,Expression,,]
#define SSCE_M_QP_BADARITHOP                    	25919		// The arithmetic operator is not recognized by SQL Server CE.  [,,,Operator,,]
#define SSCE_M_QP_TRUNCATION                    	25920		// The data is truncated. [,,,,,]
#define SSCE_M_QP_INVALIDFUNCTIONNAME           	25921		// The function is not recognized by SQL Server CE [,,,Name of function,Data type,]
#define SSCE_M_QP_INVALIDFUNCTIONARGS           	25922		// The specified argument value for the function is not valid. [,,,,,]
#define SSCE_M_QP_NOSUPPORTFORLOB               	25923		// The ntext and image data types cannot be used in WHERE, HAVING, GROUP BY, ON, or IN clauses, except when these data types are used with the LIKE or IS NULL predicates. [,,,Expression,,]
#define SSCE_M_QP_BINDTSYNTAX                   	25924		// An overflow might have occurred while converting binary to datetime.[Binary value,,,,,]
#define SSCE_M_QP_NUMDTSYNTAX                   	25925		// An overflow might have occurred while converting numeric to datetime. [Numeric value,,,,,]
#define SSCE_M_QP_MONDTSYNTAX                   	25926		// An overflow might have occurred while converting money to datetime. [Money value,,,,,]
#define SSCE_M_QP_BIGINTDTSYNTAX                	25927		// An overflow might have occurred while converting bigint to datetime. [bigint value,,,,,]
#define SSCE_M_QP_TOOMANYARGS						25928		// There are too many arguments specified for the function. [Count of arguments,,,,,]
#define SSCE_M_QP_UMINUSNUMONLY                     25929       // A unary minus can be performed only on tinyint, smallint, int, bigint, money, and numeric data types. [,,,Expression,,]
#define SSCE_M_QP_PRECISIONMISMATCH                 25930       // The operation cannot be performed because the precision of the numeric data types do not match. [,,,Expression,,]
#define SSCE_M_QP_SCALEMISMATCH                     25931       // The operation cannot be performed because the scale of the numeric data types do not match. [,,,Expression,,]
#define SSCE_M_QP_NOIMPLICITCONVALLOWED             25932       // The data types cannot be converted implicitly. [Source data type,Destination data type,,,,]
#define SSCE_M_QP_DATETIMEOVFLO                     25933       // An overflow occurred while converting to datetime. [,,,Expression,,]
#define SSCE_M_QP_MISSINGTYPELOGIC                  25934       // DEAD:Error thrown when hitting a code path that requires type specific logic that is missing. [,,,,,]
#define SSCE_M_QP_DIVBYZERO                         25935       // Dividing by zero is not allowed. [,,,Dividing by zero is not allowed.,,]
#define SSCE_M_QP_NOTIMEPARTSPECIFIED               25937       // A time datepart must be specified to convert a string to datetime. [,,,String,,]
#define SSCE_M_QP_INVALIDHOURTIMEPARTSPECIFIED      25938       // The specified hour in the time datepart is not valid. [,,,String,,]
#define SSCE_M_QP_INVALIDMINUTETIMEPARTSPECIFIED    25939       // The specified minute in the time datepart is not valid. [,,,String,,]
#define SSCE_M_QP_INVALIDSECONDIMEPARTSPECIFIED     25940       // The specified second in the time datepart is not valid. [,,,String,,]
#define SSCE_M_QP_INVALIDMSEDCONDIMEPARTSPECIFIED   25941       // The specified millisecond in the time datepart is not valid. [,,,String,,]
#define SSCE_M_QP_SYNTAXERRORINDATETIMESTRING       25942       // The specified date or time datepart is not valid. [,,,String,,]
#define SSCE_M_QP_FORMATERRORINDATETIMESTRING       25943       // The format of the specified date or time datepart is not valid. [,,,String,,]
#define SSCE_M_QP_INVALIDORDERINDATETIMESTRING      25944       // The order of the specified date or time datepart is not valid. [,,,String,,]
#define SSCE_M_QP_INVALID_ESCAPE_SEQUENCE			25945		// The escape sequence used in the LIKE predicate is not valid.  [,,,Escape sequence,Like predicate,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//

//Errors after this comment were added for v2.0 
//
#define SSCE_M_QP_INVALIDFUNCTIONARGNUM				25946		// The number of arguments specified for function <func_name> is not correct. [,,,,,]
#define SSCE_M_QP_INVALIDCONVERSION					25947		// The conversion is not supported. [,,,,,]
#define SSCE_M_QP_INVALIDFUNCTIONARGVALUE			25948		// The specified argument value for the function <func_name> is not valid. [,,,Name of function(if known),,]
#define SSCE_M_QP_INCOMPATIBLE_TYPES				25949		// The expressions in UNION are not compatible. [,,,,,]
#define SSCE_M_QP_MISSINGPARAMETER					25950		// A parameter is missing. [,,,,,]
#define SSCE_M_QP_PARAMETERNOTALLOWED				25951		// A parameter is not allowed in this location. [,,,,,]
#define SSCE_M_QP_EMPTYFROMLIST						25952		// At least one input table is required [,,,,,]


// --------------------------------------------------------------------------------------
//
// Query Storage Layer			26100 -- 26199
//	
#define SSCE_M_QP_BADTABLE     						26100		// The table name is not valid. [,,,Table name,,]
#define SSCE_M_QP_BADSCHEMAROWSET					26101		// An error occurred while accessing the schema rowset. [,,,,,]
#define SSCE_M_QP_TBLSCHEMAROWSET					26102		// An error occurred while accessing the table schema rowset.  [,,,Table name,,]
//***** Don't add any more errors here, read the comments below and add appropriately
//
// Errors after this comment were added for v1.1 (PEBBLE) 
//

// --------------------------------------------------------------------------------------
//
// Query Core 					26300 -- 26499
//	
#define SSCE_M_QP_OLEDBERR     						26300		// OLE DB returned an error. [,,,,,]
#define SSCE_M_QP_BADADT	   						26301		// DEAD:Cannot cast from the specified type [,,,,,]
#define SSCE_M_QP_BADTYPE	   						26302		// The specified data type is not valid. [Data type,,,,,]
#define SSCE_M_QP_INTERNAL	   						26303		// An internal query processor error occurred [,,,,,]
#define SSCE_M_QP_ALLWELL	   						26304		// DEAD:No error [,,,,,]
#define SSCE_M_QP_BADADTMETAINFO					26305		// The meta data is not valid. [,,,,,]
#define SSCE_M_QP_COERSIONERROR						26306		// Data conversion failed. [OLE DB status value,,,,,]
#define SSCE_M_QP_BADQUERY							26307		// Syntax error in query. [,,,,,]
#define SSCE_M_QP_ZEROLENGTHBINARYCONST				26308		// Zero-length binary constants are not supported in SQL Server CE. [,,,,,]

// ***********************************************************************
// NOTE : Our error number range is limited to the SQL Server error number 
//		  range, Which is 25000 to 30000. DO NOT ADD ANY ERRORS ABOVE OR
//		  BELOW THIS RANGE!
// ***********************************************************************

// --------------------------------------------------------------------------------------
//
// SSCERelay					30000 -- 30499 
//
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//
#define SSCE_M_WSANOTINITIALISED					30000		// The socket cannot be initialized/uninitialized.  [,,,,,]
#define SSCE_M_WSAENETDOWN							30001		// The network was unavailable, and the initialization/uninitialization of the socket failed. [,,,,,]
#define SSCE_M_WSAEINPROGRESS						30002		// The socket cannot be initialized/uninitialized. A blocking operation is currently executing.  [,,,,,]
#define SSCE_M_WSASYSNOTREADY						30003		// The socket caninitialized/uninitialized. Either a driver is missing or multiple implementations are being used. [,,,,,]
#define SSCE_M_WSAVERNOTSUPPORTED					30004		// The socket cannot be initialized/uninitialized. The current version is not supported. [,,,,,]
#define SSCE_M_WSAEPROCLIM							30005		// The socket cannot be initialized/uninitialized. The limit on the number of processes using Windows Sockets has been reached. [,,,,,]
#define SSCE_M_WSAEFAULT							30006		// An address passed to the function is not valid. [,,,,,]
#define SSCE_M_CREATEEVENTFAILED					30007		// A create event failed. For more information, see HRESULT. [,,,,,]
#define SSCE_M_SETEVENTFAILED						30008		// A set event failed. For more information, see HRESULT. [,,,,,]
#define SSCE_M_WAITFAILED							30009		// A WAITFOR event failed. For more information, see HRESULT. [,,,,,]
#define SSCE_M_INVALIDCLIENTPORT					30010		// The specified desktop computer port is not valid. [,,,,,]
#define SSCE_M_CONVERSIONTOUNICODEFAILED			30011		// The conversion of the server name to Unicode failed. [,,,,,]
#define SSCE_M_INVALIDSERVERPORT					30012		// The server port specified is not valid. [,,,,,]
#define SSCE_M_FAILUREACCESSINGSYSTEMMENU			30013		// The system menu cannot be accessed. [,,,,,]
#define SSCE_M_FAILURESETTINGREGISTRY				30014		// The registry keys cannot be set or created. [,,,,,]
#define SSCE_M_INVALIDARGUMENT						30015		// A parameter passed to SQL Server CE Relay is not valid. [,,,,,]
#define SSCE_M_INVALIDARGUMENTS						30016		// The combination of arguments is not valid. [,,,,,]
#define SSCE_M_MISSINGPORTORSERVER					30017		// Either a port number is not valid or the server name is missing. [,,,,,]
#define SSCE_M_SOCKETCALLFAILED						30018		// The socket call failed [,,,,,]
#define SSCE_M_PORTALREADYINUSE						30019		// The socket port is already in use on this device. [,,,,,]
#define SSCE_M_BINDFAILED							30020		// The bind failed. [,,,,,]
#define SSCE_M_CANTRESOLVESERVERADDRESS				30021		// The server address cannot be resolved. [,,,,,]
#define SSCE_M_LISTENFAILED							30022		// The Listen function failed on a socket. [,,,,,]
#define SSCE_M_ACCEPTFAILED							30023		// An accept from the server failed. [,,,,,]
#define SSCE_M_CONNECTFAILED						30024		// A connection request to the server has failed. [,,,,,]
#define SSCE_M_CANTCREATETHREAD						30025		// A thread cannot be created due to system problems or lack of resources. [,,,,,]
#define SSCE_M_SETKEEPALIVEFAILED					30026		// Internal error: Setting a socket option for keep alive failed. [,,,,,]
#define SSCE_M_SETNAGLEFAILED						30027		// Internal error: Setting a socket option for the Nagle algorithm failed. [,,,,,]
#define SSCE_M_RECEIVEFAILED						30028		// The receive operation from the socket failed. [,,,,,]
#define SSCE_M_SENDFAILED							30029		// The send operation to the socket failed. [,,,,,]
#define SSCE_M_INITARGSFAILED						30030		// The initialization of arguments failed.[,,,,,]
#define SSCE_M_WSASTARTUPFAILED						30031		// The WSAStartup function failed during socket initialization. [,,,,,]
#define SSCE_M_WSACLEANUPFAILED						30032		// The WSACleanup function failed during a cleanup operation. [,,,,,]
#define SSCE_M_INVALIDSERVERNAME					30033		// The server name is either not valid or not specified. [,,,,,]
#define SSCE_M_CONNECTIONRESET						30034		// The socket connection was reset. [,,,,,]


// --------------------------------------------------------------------------------------
//
// Miscellaneous					39500 -- 39999 
//
//
//Errors after this comment were added for v1.1 (PEBBLE) 
//
#define SSCE_M_EVALUATIONCOPYEXPIRED				39999		// The evaluation copy has expired. [,,,,,]


// *************************************************************************************
// NOTE: THIS FILE IS PARSED BY SQLDRIVER - PLEASE DO THE FOLLOWING:
// *************************************************************************************
//
// 1. DO NOT put an error (or description) on more than one line
// 2. DO NOT include " in the description - this screws things up 
// 3. Make sure the comment is at the end of a line and has a space 
//    before the text ("// ")
// 4. All Errors should document error parameters after the error string in this format [#,#,#,s,s,s]
//		[,,,,,] == no error parameters for this error. 
//		[...] == we need to fix up use of this error - parameters are inconsitent
//		(nothing) == this error hasn't had its parameters recorded in this header file yet.
//                   It DOES NOT mean there are no parameters!
// 5. Our error number range is limited to the SQL Server error number range, Which is 25000 to 30000. 
//    DO NOT ADD ANY ERRORS ABOVE OR BELOW THIS RANGE!
// 6. If you add errors to this file, you must also add the error and the string to 
//    SSCE\SSCEError\SSCEError20xx.rc. The xx denotes the language for localization. (en = US English)
// *************************************************************************************


#endif // SSCEERR_INCLUDED
