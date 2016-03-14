#include <config.h>
#include <page_manager.h>
#include <disp_manager.h>
#include <stdlib.h>
#include <render.h>

static void MainPageRun(PT_PageParams ptParentPageParams);
static int MainPageGetInputEvent(PT_PageLayout ptPageLayout,PT_InputEvent ptInputEvent);
static int MainPagePrepare(void);

static T_Layout g_atMainPageLayout[] = {
    {0,0,0,0,"browse_mode.bmp"},
    {0,0,0,0,"continue_mod.bmp"},
    {0,0,0,0,"setting.bmp"},
    {0,0,0,0,NULL},
};

static T_PageLayout g_tMainPageLayout = {
	.iMaxTotalBytes = 0,
	.atLayout       = g_atMainPageLayout,
};

static T_PageAction g_tMainPageAction = {
    .name           = "main",
    .Run            = MainPageRun,
    .GetInputEvent  = MainPageGetInputEvent,
    .Prepare        = MainPagePrepare,
};



/* �����ͼ������ֵ */
static void  CalcMainPageLayout(PT_PageLayout ptPageLayout)
{
	int iStartY;
	int iWidth;
	int iHeight;
	int iXres, iYres, iBpp;
	int iTmpTotalBytes;
	PT_Layout atLayout;

	atLayout = ptPageLayout->atLayout;
	GetDispResolution(&iXres, &iYres, &iBpp);
	ptPageLayout->iBpp = iBpp;

	/*
	 *    ----------------------
	 *                           1/2 * iHeight
	 *          browse_mode.bmp  iHeight
	 *                           1/2 * iHeight
	 *         continue_mod.bmp     iHeight
	 *                           1/2 * iHeight
	 *          setting.bmp       iHeight
	 *                           1/2 * iHeight
	 *    ----------------------
	 */

	iHeight = iYres * 2 / 10;
	iWidth  = iHeight;
	iStartY = iHeight / 2;

	/* select_foldͼ�� */
	atLayout[0].iTopLeftY  = iStartY;
	atLayout[0].iBotRightY = atLayout[0].iTopLeftY + iHeight - 1;
	atLayout[0].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[0].iBotRightX = atLayout[0].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1) * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}


	/* intervalͼ�� */
	atLayout[1].iTopLeftY  = atLayout[0].iBotRightY + iHeight / 2 + 1;
	atLayout[1].iBotRightY = atLayout[1].iTopLeftY + iHeight - 1;
	atLayout[1].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[1].iBotRightX = atLayout[1].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[1].iBotRightX - atLayout[1].iTopLeftX + 1) * (atLayout[1].iBotRightY - atLayout[1].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

	/* returnͼ�� */
	atLayout[2].iTopLeftY  = atLayout[1].iBotRightY + iHeight / 2 + 1;
	atLayout[2].iBotRightY = atLayout[2].iTopLeftY + iHeight - 1;
	atLayout[2].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[2].iBotRightX = atLayout[2].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[2].iBotRightX - atLayout[2].iTopLeftX + 1) * (atLayout[2].iBotRightY - atLayout[2].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

}



static void ShowMainPage(PT_PageLayout ptPageLayout)
{
    PT_VideoMem ptVideoMem;
    int iError;

    PT_Layout atLayout = ptPageLayout->atLayout;

    /* 1. ����Դ� */
    ptVideoMem = GetVedioMem(ID("main"), 1);
    if (ptVideoMem == NULL)
    {
        DBG_PRINTF("can't get video mem for main page!\n");
        return;
    }

    /* 2. �軭���� */

    /* �����û�м������ͼ������� */
    if (atLayout[0].iTopLeftX == 0)
    {
        CalcMainPageLayout(ptPageLayout);
    }

    iError = GeneratePage(ptPageLayout, ptVideoMem);

    /* 3. ˢ���豸��ȥ */
    FlushVideoMemToDev(ptVideoMem);

    /* 4. ����Դ� */
    PutVideoMem(ptVideoMem);
}


static void MainPageRun(PT_PageParams ptParentPageParams)
{
    int iIndex;
    T_InputEvent tInputEvent;
    int iBepressed = 0;
    int iIndexPressed = -1;


    T_PageParams tPageParams;

    tPageParams.iPageID = ID("main");
    /*��ʾҳ��*/
    ShowMainPage(&g_tMainPageLayout);
    /*����prepare�߳�*/

    /*����GetinputEvent��������¼�����������*/
    while(1){
        iIndex = MainPageGetInputEvent(&g_tMainPageLayout,&tInputEvent);
        if(tInputEvent.iPressure == 0)
        {
            /*����ɿ�*/
            if(iBepressed)
            {
                /*�����а�ť������*/
                ReleaseButton(&g_atMainPageLayout[iIndexPressed]);
                iBepressed = 0;
                if(iIndexPressed == iIndex) /*���º��ɿ�����ͬһ����ť*/
                {
                    switch (iIndexPressed){

					case 0: /* �����ť */
					{
                        tPageParams.strCurPictureFile[0] = '\0';
                        Page("manual")->Run(&tPageParams);
						/* ������ҳ�淵�غ���ʾ���׵���ҳ�� */
						ShowMainPage(&g_tMainPageLayout);

						break;
					}
                    case 1: /* ������ť */
					{
                        Page("auto")->Run(&tPageParams);

						/* ������ҳ�淵�غ���ʾ���׵���ҳ�� */
						ShowMainPage(&g_tMainPageLayout);

						break;
					}
                    case 2:/*���ð�ť */{

                        Page("setting")->Run(&tPageParams);
                        /* ������ҳ�淵�غ���ʾ���׵���ҳ�� */
                        ShowMainPage(&g_tMainPageLayout);
                    }
                        break;
                    default:
                        break;
                    }
                    iIndexPressed = -1;
                }
            }
            else{
                /*����״̬*/
                if(iIndex != -1){
                   if(!iBepressed){
                        /*��һ�ΰ���*/
                        iBepressed = 1;
                        iIndexPressed = iIndex;
                        PressButton(&g_atMainPageLayout[iIndexPressed]);
                   }
                }
            }
        }
    }
}

static int MainPagePrepare(void)
{
    return 0;
}

static int MainPageGetInputEvent(PT_PageLayout ptPageLayout,PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);

}

int MainPageInit(void)
{
    return RegisterPageAction(&g_tMainPageAction);
}