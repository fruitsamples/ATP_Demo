/*****************************************************************	Program:	< ATP Demo >	File:		< ATP Demo.h >		Written by  Scott Kuechle	of <Apple Macintosh Developer Technical Support>		10/92 SRK created	8/94 SRK Modified to use a queue of parameter	         blocks.	Copyright � 1992, 1994 Apple Computer, Inc.	All rights reserved.	*****************************************************************/#include	<types.h>#include	<quickdraw.h>#include	<toolutils.h>#include	<fonts.h>#include	<events.h>#include	<windows.h>#include	<dialogs.h>#include	<menus.h>#include	<desk.h>#include	<textedit.h>#include	<scrap.h>#include	<segload.h>#include	<osevents.h>#include	<files.h>#include	<devices.h>#include	<memory.h>#include	<appletalk.h>#include	<lists.h>#include	<SysEqu.h>#include	<Script.h>#include	<CursorCtl.h>#include	<Resources.h>#include	<ADSP.h>#include	<Packages.h>#include	<String.h>#include	<Strings.h>#include	<Traps.h>#include	<GestaltEqu.h>#include	<Errors.h>typedef short SICN[16];typedef SICN *SICNList;typedef SICNList *SICNHand;/* 1.01 - kMinHeap - This is the minimum result from the following	 equation:						ORD(GetApplLimit) - ORD(ApplicZone)				 for the application to run. It will insure that enough memory will	 be around for reasonable-sized scraps, FKEYs, etc. to exist with the	 application, and still give the application some 'breathing room'.	 To derive this number, we ran under a MultiFinder partition that was	 our requested minimum size, as given in the 'SIZE' resource. */	 #define		kMinHeap				(29 * 1024)#define		kMinSpace				(20 * 1024)#define		_WaitNextEvent			0xA860#define		_Unimplemented			0xA89F/* For System 7.0 */#define 	_Gestalt 				0xA1AD#define		appleID					128 			/* This is a resource ID */#define		fileID 					129 			/* ditto */#define		editID 					130 			/* ditto */#define 	extremeNeg				-32768#define 	extremePos				32767 - 1			/* required for old region bug */#define		osEvent					app4Evt		/* event used by MultiFinder */#define		resumeMask				1			/* bit of message field for resume vs. suspend */#define		sysEnvironsVersion		1#define		kATPTimeOutVal			3			/* re-try ATP SendRequest every 3 seconds */#define		kATPRetryCount			5			/* for five times */#define		kZonesSize				578			/* size of buffer for zone names */#define		kGetZoneList			8			/* the Zone Information Protocol socket */#define		kZIPSocket				6			/* the Zone Information Protocol socket */#define		kMoreZones				0xFF000000 	/* mask to see if more zones to come */#define		kZoneCount				0x0000FFFF 	/* mask to count zones in buffer */#define		kHilite					1			/* hilite value for button control */#define		kDeHilite				0			/* dehilite value for button control */#define 	theBridgeSocket 		0x6/*  *	constants for window activation  */#define 	kActivateWindow			1#define 	kDeactivateWindow		0#define		maxZones				250				/* max. number of zones we can save in our buffer */#define		ZoneBufSize				maxZones * 33	/* our ugly, mondo zone buffer */#define		BigLookupBuffer			10000			/* another big, ugly buffer - for nbp lookups */// ------------------------------------------------#define 	rMenuBar 				128					/*application's menu bar*/#define		ZoneMenuID				220#define		TypeMenuID				221#define		ObjectMenuID			222#define		DataSizeMenuID			223#define 	rAboutDialog  			128					/*about alert*/#define 	rDialog  				129					/*application's window*/#define 	rErrorDialog  			130					/*error alert window*/	/* The following constants are used to identify menus and their items. The menu IDs*/	/* have an "m" prefix and the item numbers within each menu have an "i" prefix.*/#define 	mApple  				128					/*Apple menu*/#define 	iAbout  				1#define 	mFile  					129					/*File menu*/#define 	iClose  				1#define 	iQuit  					2#define 	mEdit  					130					/*Edit menu*/#define 	iUndo  					1#define 	iCut  					3#define 	iCopy  					4#define 	iPaste  				5#define 	iClear  				6#define 	mRequestOptions  		131					/*Request Options menu*/#define 	iReqClockTime  			1#define 	iReqData  				2#define 	iSingleRequest  		4#define 	iMultipleRequests  		5#define 	kStandardTriSICN  		-3990/* dialog items */#define 	kobjectItemID 			1#define 	ktypeItemID  			2#define 	kMoofFilterCheckBox		3#define 	kzoneItemID  			4#define 	kPopupBorderID			8#define 	kClockTime				9#define 	kClockTimeBorder		11#define 	kReqDataButton			10#define 	kStatusText				12/* our error message codes */#define 	atalkErr				1#define 	memErr					2#define 	menuErr					3#define 	nbpErr					4#define 	noTargetErr				5#define 	badROMsErr				6#define 	heapErr					7#define 	noMemErr				8#define 	DrvrErr					9#define 	SktErr					10#define 	RsrcErr					11#define 	dataNotValidErr			12#define 	dataIsValid				13	/* our test data resource definitions */#define 	kTestDataRsrcID			128#define 	kTestDataType			'test'/* defines for req/resp packet */#define 	maxQElements			10	/* our request command codes */#define		kSendTime				1#define		kSendData				2#define		kMaxPacketSize			578		/* max. size for atp data is 578 bytes */#define		kMaxResponses			8		/* maximum number of responses we expect */#define		kRespBufSize			kMaxPacketSize * kMaxResponses	/* our response buffer */#define		kSendReqBtnText			"\pSend Request"#define		kStopReqBtnText			"\pStop Requests"#define		kTestDataCorrect		"\pTest data received is correct!"#define		kTestDataIncorrect		"\pTest data received is not correct."#define		kBlankText				"\p                                  "#define		kMaxBDSElements			8	/* from the header file def. for BDSElement */struct ourRespBuf{	char buf[kRespBufSize];};typedef struct ourRespBuf ourRespBuf;struct ourReqData{	char data[kMaxPacketSize];};typedef struct ourReqData ourReqData;#define myPrivateFields \	OSErr			functionResult;\	Ptr				reqData;\	Ptr				respData;\	Ptr				bdsPtr;	\	long 			myA5;\	ProcPtr			ourCompletion;struct ourParamBlockFields{	myPrivateFields};typedef struct ourParamBlockFields ourParamBlockFields;	#define	kOurPBNegOffset	sizeof(ourParamBlockFields)	/* our parameter block structure - Note that		the "myA5" and "ourCompletion" fields must		remain 4 & 8 bytes above the atp pb. If you		change this, you must change our assembly		routine "PreCompletion" */struct myATPParamBlock{	myPrivateFields	ATPParamBlock 	u;};typedef struct myATPParamBlock myATPParamBlock;typedef struct myATPParamBlock *myATPParamBlockPtr;#define	atpEntityFilter		"\patp"