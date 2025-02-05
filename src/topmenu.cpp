// =============================================================================
// This file is part of boinctui.
// http://boinctui.googlecode.com
// Copyright (C) 2012,2013 Sergey Suslov
//
// boinctui is free software; you can redistribute it and/or modify it  under
// the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// boinctui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details
// <http://www.gnu.org/licenses/>.
// =============================================================================

#include <string.h>
#include "topmenu.h"
#include "tuievent.h"
#include "kclog.h"


//Названия пунктов верхнего меню
#define M_FILE 				"File"
#define M_VIEW 				"View"
#define M_PROJECTS			"Projects"
#define M_TASKS				"Tasks"
#define M_ACTIVITY			"Activity"
#define M_OPTIONS			"Options"
#define M_HELP				"Help"
//Названия пунктов меню "File"
#define M_NEXT_HOST			"Next BOINC host"
#define M_CONFIG_HOSTS			"Configure host list"
#define M_VIEW_STATS			"View Statistics"
#define M_RUN_BENCHMARKS		"Run CPU benchmarks"
#define M_QUIT				"Quit boinctui"
//Названия пунктов меню "View"
#define M_VIEW_NUMBER			"Task number column"
#define M_VIEW_STATE			"Task state column"
#define M_VIEW_DONE			"Percent done column"
#define M_VIEW_PROJECT			"Project name column"
#define M_VIEW_ESTIMATE			"Estimate time column"
#define M_VIEW_DEADLINE			"Deadline time column"
#define M_VIEW_APPNAME			"Application name column"
#define M_VIEW_TASKNAME			"Task name column"
#define M_ALL_TASKS			"All tasks"
#define M_HIDE_DONE			"Hide done tasks"
#define M_ACTIVE_ONLY			"Active tasks only"
#define M_UNSORTED			"Unsorted tasks list"
#define M_SORT_BY_STATE			"Sort by state"
#define M_SORT_BY_DONE			"Sort by done %"
#define M_SORT_BY_PROJECT		"Sort by project name"
#define M_SORT_BY_ESTIMATE		"Sort by estimate time"
#define M_SORT_BY_DL			"Sort by deadline"
#define M_SORT_BY_APP			"Sort by application"
#define M_SORT_BY_TASK			"Sort by task name"
#define M_ASCII_LINE_DRAW		"ASCII line draw"
//Названия пунктов меню "Projects"
#define M_ADD_PROJECT			"Add project"
#define M_CONNECT_MANAGER		"Connect to account manager"
#define M_SYNCHRONIZE_MANAGER		"Synchronize with manager"
#define M_DISCONNECT_MANAGER		"Stop using account manager"
//Меню списка менеджеров
#define M_ADD_NEW_MGR			"Add new account manager"
//Названия пунктов подменю "Projects/Имя проекта"
#define M_UPDATE_PROJECT		"Update project"
#define M_SUSPEND_PROJECT		"Suspend project"
#define M_RESUME_PROJECT		"Resume project"
#define M_NO_NEW_TASKS_PROJECT		"No new tasks"
#define M_ALLOW_NEW_TASKS_PROJECT	"Allow new tasks"
#define M_RESET_PROJECT			"Reset project"
#define M_DETACH_PROJECT		"Detach project"
//Названия пунктов меню "Add project/User Exist"
#define M_PROJECT_USER_EXIST		"Existing user"
#define M_PROJECT_NEW_USER		"Create new user account"
//Названия пунктов меню "Tasks"
#define M_SUSPEND_TASK			"Suspend task"
#define M_RESUME_TASK			"Resume task"
#define M_ABORT_TASK			"Abort task"
#define M_INFO_TASK			"View raw task info"
//Названия пунктов меню "Activity"
#define M_ACTIVITY_ALWAYS		"Run always"
#define M_ACTIVITY_AUTO			"Run based on preferences"
#define M_ACTIVITY_NEVER		"Suspend"
#define M_GPU_ACTIVITY_ALWAYS		"GPU run always"
#define M_GPU_ACTIVITY_AUTO		"GPU run based on preferences"
#define M_GPU_ACTIVITY_NEVER		"GPU suspend"
#define M_NET_ACTIVITY_ALWAYS		"Network activity always available"
#define M_NET_ACTIVITY_AUTO		"Network activity based on preferences"
#define M_NET_ACTIVITY_NEVER		"Network activity suspend"
//Defines menu "Options"
#define M_PREFERENCES			"Options"
#define M_READ_CONFIG_FILES		"Read config files"

//Названия пунктов меню "Help"
#define M_ABOUT				"About"
#define M_KEY_BINDINGS			"Hot keys list"


TopMenu::TopMenu(/*Config* cfg*/) : NMenu(NRect(1,getmaxx(stdscr),0,0),true)
{
    setserver(NULL);
    //this->cfg = cfg;
    additem(M_FILE,"");
    additem(M_VIEW,"");
    additem(M_PROJECTS,"");
    additem(M_TASKS,"");
    additem(M_ACTIVITY,"");
    additem(M_OPTIONS,"");	
    additem(M_HELP,"");
    additem(NULL,NULL);
    setbackground(getcolorpair(COLOR_WHITE,COLOR_BLUE)|A_BOLD);
    enableflag = true;
    disable();
}


bool TopMenu::action() //open the submenu
{
    bool result = false;
    if (!items.empty()) //If we're already open layout
	return false;
    //создаем подменю
    int begincol = (menu->itemlen+menu->spc_cols)*item_index(current_item(menu)) + menu->spc_cols; //смещение на экране по горизонтали
    if ( strcmp(item_name(current_item(menu)),M_FILE) == 0 )
    {
	insert(new FileSubMenu(NRect(5,25,1, begincol)));
	result = true;
    }
    if ( strcmp(item_name(current_item(menu)),M_VIEW) == 0 )
    {
	insert(new ViewSubMenu(NRect(5,25,1, begincol)/*, cfg*/));
	result = true;
    }
    if ( strcmp(menu->curitem->name.str,M_PROJECTS) == 0 )
    {
	insert(new ProjectsSubMenu(NRect(5,25,1, begincol), srv));
	result = true;
    }
    if ( strcmp(item_name(current_item(menu)),M_TASKS) == 0 )
    {
	insert(new TasksSubMenu(NRect(5,25,1, begincol)));
	result = true;
    }
    if ( strcmp(item_name(current_item(menu)),M_ACTIVITY) == 0 )
    {
	insert(new ActivitySubMenu(NRect(5,25,1, begincol), srv));
	result = true;
    }
    if ( strcmp(item_name(current_item(menu)),M_HELP) == 0 )
    {
	insert(new HelpSubMenu(NRect(5,25,1, begincol)));
	result = true;
    }
    return result;
}


void TopMenu::eventhandle(NEvent* ev) 	//обработчик событий
{
    if ( !isenable() /*&& ( ev->type != NEvent::evMOUSE)*/ )
    {
	if ( ev->type == NEvent::evMOUSE)
	{
	    NMouseEvent* mevent = (NMouseEvent*)ev;
	    if ((mevent->cmdcode & BUTTON1_CLICKED) && (isinside(mevent->row, mevent->col)))
		enable(); //влючить по клику
	}
	else
	    return; //не обрабатывать клавиатурные если меню не активно
    }
    if ( ev->done )
	return; //не обрабатывать если уже обработано кем-то ранее
    //отправляем событие всем подменю
    NMenu::eventhandle(ev); //предок
    if ( ev->done )
	return; //выход если какое-то подменю отработало событие
    //пытаемся обработать самостоятельно
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case KEY_ENTER:
	    case '\n':
		if (!createsubmenu())
		    ev->done = false; //пусть обрабатывает владелец
		break;
	    case KEY_UP: 	//открыть подменю по стрелкевверх
	    case KEY_DOWN: 	//открыть подменю по стрелкевниз
		action();
		break;
	    case 27:
		if ( !items.empty() )
		    destroysubmenu();
		else
		    ev->done = false; //пусть обрабатывает владелец
		break;
	    case KEY_RIGHT:
		menu_driver(menu, REQ_RIGHT_ITEM);
		if ( !items.empty() ) //подменю открыто
		{
		    destroysubmenu(); //закрыть старое
		    action(); //открыть новое
		}
		break;
	    case KEY_LEFT:
		menu_driver(menu, REQ_LEFT_ITEM);
		if ( !items.empty() ) //подменю открыто
		{
		    destroysubmenu(); //закрыть старое
		    action(); //открыть новое
		}
		break;
	    default:
		ev->done = false; //нет реакции на этот код
	} //switch
    }
    //если клик мыши до сих пор не обработан
    //значит он за пределами меню и меню нужно гасить
    if ( ev->type == NEvent::evMOUSE)
    {
	NMouseEvent* mevent = (NMouseEvent*)ev;
	if ((mevent->cmdcode & (BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED)) && (!ev->done))
	    disable(); //выключить по клику вне меню
    }
    if (ev->done) //если обработали, то нужно перерисоваться
        refresh();
}


//=============================================================================================


FileSubMenu::FileSubMenu(NRect rect) : NMenu(rect)
{
    additem(M_NEXT_HOST,"  N");
    additem(M_CONFIG_HOSTS,"  C");
    additem(M_VIEW_STATS,"  V");
    additem(M_RUN_BENCHMARKS,"");
    additem(M_QUIT,"  Q");
    additem(NULL,NULL);
}


bool FileSubMenu::action()
{
    putevent(new NEvent(NEvent::evKB, KEY_F(9))); //закрыть осн меню
    if ( strcmp(item_name(current_item(menu)),M_NEXT_HOST) == 0 )
	putevent(new NEvent(NEvent::evKB, 'N')); //создаем событие иммитирующее нажатие 'N'
    if ( strcmp(item_name(current_item(menu)),M_CONFIG_HOSTS) == 0 )
	putevent(new NEvent(NEvent::evKB, 'C')); //создаем событие иммитирующее нажатие 'C'
    if ( strcmp(item_name(current_item(menu)),M_VIEW_STATS) == 0 )
	putevent(new NEvent(NEvent::evKB, 'V')); //создаем событие иммитирующее нажатие 'V'
    if ( strcmp(item_name(current_item(menu)),M_RUN_BENCHMARKS) == 0 )
	putevent(new TuiEvent(evBENCHMARK)); //NEvent(NEvent::evPROG, 5)); //создаем событие запускающее бенчмарк
    if ( strcmp(item_name(current_item(menu)),M_QUIT) == 0 )
	putevent(new NEvent(NEvent::evKB, 'Q')); //создаем событие иммитирующее нажатие 'Q'
    return true;
}


//=============================================================================================


ViewSubMenu::ViewSubMenu(NRect rect/*, Config* cfg*/) : NMenu(rect)
{
    int colnum = 0;
    additem(M_VIEW_NUMBER, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem(M_VIEW_STATE, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem(M_VIEW_DONE, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem(M_VIEW_PROJECT, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem(M_VIEW_ESTIMATE, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem(M_VIEW_DEADLINE, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem(M_VIEW_APPNAME, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem(M_VIEW_TASKNAME, iscolenable(/*cfg,*/colnum++) ? "[*]" : "[ ]");
    additem("","");
    int taskslistmode = 0;
    if (gCfg != NULL)
    {
	Item* rootcfg = gCfg->getcfgptr();
	if (rootcfg != NULL)
	{
	    Item* tasks_list_mode = rootcfg->findItem("tasks_list_mode");
	    taskslistmode = tasks_list_mode->getivalue();
	}
    }
    additem(M_ALL_TASKS,   (taskslistmode == 0) ? "(*)" : "( )");
    additem(M_HIDE_DONE,   (taskslistmode == 1) ? "(*)" : "( )");
    additem(M_ACTIVE_ONLY, (taskslistmode == 2) ? "(*)" : "( )");
    additem("","");
    int taskssortmode = 0;
    if (gCfg != NULL)
    {
	Item* rootcfg = gCfg->getcfgptr();
	if (rootcfg != NULL)
	{
	    Item* tasks_sort_mode = rootcfg->findItem("tasks_sort_mode");
	    taskssortmode = tasks_sort_mode->getivalue();
	}
    }
    additem(M_UNSORTED,         (taskssortmode == 0) ? "(*)" : "( )");
    additem(M_SORT_BY_STATE,    (taskssortmode == 1) ? "(*)" : "( )");
    additem(M_SORT_BY_DONE,     (taskssortmode == 2) ? "(*)" : "( )");
    additem(M_SORT_BY_PROJECT,  (taskssortmode == 3) ? "(*)" : "( )");
    additem(M_SORT_BY_ESTIMATE, (taskssortmode == 4) ? "(*)" : "( )");
    additem(M_SORT_BY_DL,       (taskssortmode == 5) ? "(*)" : "( )");
    //additem(M_SORT_BY_APP,      (taskssortmode == 6) ? "(*)" : "( )");  //not implemented yet
    additem(M_SORT_BY_TASK,     (taskssortmode == 7) ? "(*)" : "( )");
    additem("","");
    additem(M_ASCII_LINE_DRAW,  (asciilinedraw == 1) ? "[*]" : "[ ]");
    additem(NULL,NULL);
}


bool ViewSubMenu::iscolenable(/*Config* cfg,*/ int n)
{
    bool result = false;
    //читаем из конфига
    if (gCfg != NULL)
    {
	Item* rootcfg = gCfg->getcfgptr();
	if (rootcfg != NULL)
	{
	    Item* column_view_mask = rootcfg->findItem("column_view_mask");
	    unsigned int columnmask = column_view_mask->getivalue();
	    result = (1 << n) & columnmask;
	}
    }
    return result;
}


bool ViewSubMenu::action()
{
    putevent(new NEvent(NEvent::evKB, KEY_F(9))); //закрыть осн меню

    if ( strcmp(item_name(current_item(menu)),M_ALL_TASKS) == 0 )
    {
	putevent(new TuiEvent(evVIEWMODECH, 0));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)),M_HIDE_DONE) == 0 )
    {
	putevent(new TuiEvent(evVIEWMODECH, 1));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)),M_ACTIVE_ONLY) == 0 )
    {
	putevent(new TuiEvent(evVIEWMODECH, 2));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_UNSORTED) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 0));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_SORT_BY_STATE) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 1));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_SORT_BY_DONE) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 2));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_SORT_BY_PROJECT) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 3));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_SORT_BY_ESTIMATE) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 4));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_SORT_BY_DL) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 5));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_SORT_BY_APP) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 6));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_SORT_BY_TASK) == 0 )
    {
	putevent(new TuiEvent(evSORTMODECH, 7));
	return true;
    }

    if ( strcmp(item_name(current_item(menu)), M_ASCII_LINE_DRAW) == 0 )
    {
	asciilinedraw = !asciilinedraw;
	putevent(new TuiEvent(evASCIIMODECHANGE));
	return true;
    }

    putevent(new TuiEvent(evCOLVIEWCH,item_index(current_item(menu)), false));
    return true;
}


//=============================================================================================


HelpSubMenu::HelpSubMenu(NRect rect) : NMenu(rect)
{
    additem(M_ABOUT,"");
    additem(M_KEY_BINDINGS,"");
    additem(NULL,NULL);
}


bool HelpSubMenu::action()
{
    putevent(new NEvent(NEvent::evKB, KEY_F(9))); //закрыть осн меню
    if ( strcmp(item_name(current_item(menu)),M_ABOUT) == 0 )
	putevent(new TuiEvent(evABOUT)); //NEvent(NEvent::evPROG, 3)); //создаем событие с кодом 3 "окно About"
    if ( strcmp(item_name(current_item(menu)),M_KEY_BINDINGS) == 0 )
	putevent(new TuiEvent(evKEYBIND));//NEvent(NEvent::evPROG, 4)); //создаем событие с кодом 4 "окно Key Bindings"
    return true;
}


//=============================================================================================


ProjectsSubMenu::ProjectsSubMenu(NRect rect, Srv* srv) : NMenu(rect)
{
    this->srv = srv;
    std::string acctmgrname = "";
    if (srv != NULL)
    {
	if (!srv->acctmgrinfodom.empty())
	{
	    Item* tmpacctmgrinfodom = srv->acctmgrinfodom.hookptr();
	    Item* acct_mgr_name = tmpacctmgrinfodom->findItem("acct_mgr_name");
	    if (acct_mgr_name != NULL)
		acctmgrname = acct_mgr_name->getsvalue();
	    Item* acct_mgr_url = tmpacctmgrinfodom->findItem("acct_mgr_url");
	    if (acct_mgr_url != NULL)
		accmgrurl = acct_mgr_url->getsvalue();
	    srv->acctmgrinfodom.releaseptr(tmpacctmgrinfodom);
	}
    }
    additem(M_ADD_PROJECT,"");
    if (acctmgrname.empty())
	additem(M_CONNECT_MANAGER,"");
    else
    {
	additem(M_SYNCHRONIZE_MANAGER,acctmgrname.c_str());
	additem(M_DISCONNECT_MANAGER,acctmgrname.c_str());
    }
    //имена подключенных проектов
    additem("","");
    if ((srv != NULL)&&(!srv->statedom.empty()))
    {
	Item* tmpstatedom = srv->statedom.hookptr();
	Item* client_state = tmpstatedom->findItem("client_state");
	if (client_state != NULL)
	{
	    std::vector<Item*> projects = client_state->getItems("project");
	    for (int i = 0; i < projects.size(); i++)
	    {
		Item* project_name = projects[i]->findItem("project_name");
		if (project_name != NULL)
		{
		    std::string status = ""; //строка состояния
		    if (projects[i]->findItem("suspended_via_gui") != NULL)
			status = status + "[Susp.]";
		    if (projects[i]->findItem("dont_request_more_work") != NULL)
			status = status + "[N.N.Tsk]";
		    additem(project_name->getsvalue(),status.c_str());
		}
	    }
	}
	srv->statedom.releaseptr(tmpstatedom);
    }
    additem(NULL,NULL);
}


bool ProjectsSubMenu::action()
{
    bool result = false;
    if (items.size() > 1) //если уже открыто выходим (>1 из-за скроллбара)
	return false;
    //формируем код операции для подменю
    bool actiondone = false;
    //создаем подменю
    int begincol = 2/*getwidth() - 2*/; //смещение на экране по горизонтали
    int beginrow = 2 + item_index(current_item(menu)); //смещение на экране по вертикали
    if ( strcmp(item_name(current_item(menu)), M_ADD_PROJECT) == 0 ) //подключиться к новому проекту
    {
	insert(new ProjectAllListSubMenu(NRect(5,25,beginrow, begincol), srv));
	actiondone = true;
    }
    if ( strcmp(item_name(current_item(menu)), M_CONNECT_MANAGER) == 0 ) //подключить менеджер
    {
	insert(new ProjectAccMgrSubMenu(NRect(5,25,beginrow, begincol), srv));
	actiondone = true;
    }
    if ( strcmp(item_name(current_item(menu)), M_DISCONNECT_MANAGER) == 0 ) //отключить менеджер
    {
	putevent(new NEvent(NEvent::evKB, KEY_F(9))); //закрыть осн меню
	std::string errmsg;
	if (srv != NULL)
	    srv->accountmanager("","","",false,errmsg);
	actiondone = true;
    }
    if ( strcmp(item_name(current_item(menu)), M_SYNCHRONIZE_MANAGER) == 0 ) //синхронизироваться с менеджером
    {
	putevent(new NEvent(NEvent::evKB, KEY_F(9))); //закрыть осн меню
	std::string errmsg;
	if (srv != NULL)
	    srv->accountmanager(accmgrurl.c_str(),"","",true,errmsg);
	actiondone = true;
    }
    //если ни одна M_ константа не подошла значит это название проекта
    if (!actiondone)
    {
	insert(new ProjectListSubMenu(NRect(5,25,beginrow, begincol), srv, item_name(current_item(menu))));
    }
    return true;
}


void ProjectsSubMenu::eventhandle(NEvent* ev) 	//обработчик событий
{
    if ( ev->done )
	return;
    NMenu::eventhandle(ev); //предок
    if ( ev->done )
	return;
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case 27:
		if ( items.size() > 1 ) //1 из-за скроллбара
		    destroysubmenu();
		else
		    ev->done = false; //пусть обрабатывает владелец
		break;
	    default:
		ev->done = false; //нет реакции на этот код
	} //switch
	if (ev->done) //если обработали, то нужно перерисоваться
	    refresh();
    }
}


//=============================================================================================


TasksSubMenu::TasksSubMenu(NRect rect) : NMenu(rect)
{
    additem(M_SUSPEND_TASK,"  S");
    additem(M_RESUME_TASK ,"  R");
    additem(M_ABORT_TASK  ,"  A");
    additem(M_INFO_TASK  ,"  Enter");
    additem(NULL,NULL);
}


bool TasksSubMenu::action()
{
    putevent(new NEvent(NEvent::evKB, KEY_F(9))); //закрыть осн меню
    if ( strcmp(item_name(current_item(menu)),M_SUSPEND_TASK) == 0 )
	putevent(new NEvent(NEvent::evKB, 'S')); //создаем событие иммитирующее нажатие 'S'
    if ( strcmp(item_name(current_item(menu)),M_RESUME_TASK) == 0 )
	putevent(new NEvent(NEvent::evKB, 'R')); //создаем событие иммитирующее нажатие 'R'
    if ( strcmp(item_name(current_item(menu)),M_ABORT_TASK) == 0 )
    {
	TuiEvent* ev = new TuiEvent(evABORTRES);
	ev->bdata1 = false;
	putevent(ev); //NEvent(NEvent::evPROG, 2)); //создаем событие с кодом 2 "abort_result"
    }
    if ( strcmp(item_name(current_item(menu)),M_INFO_TASK) == 0 )
	putevent(new NEvent(NEvent::evKB, KEY_ENTER)); //создаем событие имитирующее нажатие 'Enter'
    return true;
}


//=============================================================================================


ActivitySubMenu::ActivitySubMenu(NRect rect, Srv* srv) : NMenu(rect)
{
    unpost_menu(menu);
    this->srv = srv;
    if ((srv != NULL)&&(!srv->ccstatusdom.empty()))
    {
	Item* tmpccstatusdom = srv->ccstatusdom.hookptr();
	Item* task_mode = tmpccstatusdom->findItem("task_mode");
	additem(M_ACTIVITY_ALWAYS, ((task_mode!=NULL)&&(task_mode->getivalue() == 1)) ? "(*)" : "( )"); //1 always
	additem(M_ACTIVITY_AUTO,((task_mode!=NULL)&&(task_mode->getivalue() == 2)) ? "(*)" : "( )"); 	//2 pref
	additem(M_ACTIVITY_NEVER,((task_mode!=NULL)&&(task_mode->getivalue() == 3)) ? "(*)" : "( )"); 	//3 never
	if (!srv->statedom.empty())
	{
	    Item* tmpstatedom = srv->statedom.hookptr();
	    Item* have_ati = tmpstatedom->findItem("have_ati");
	    Item* have_cuda = tmpstatedom->findItem("have_cuda");
	    if ( (have_cuda != NULL)||(have_ati !=NULL) )
	    {
		Item* gpu_mode =  tmpccstatusdom->findItem("gpu_mode");
		additem("",""); //delimiter
		additem(M_GPU_ACTIVITY_ALWAYS, ((gpu_mode!=NULL)&&(gpu_mode->getivalue() == 1)) ? "(*)" : "( )"); //1 always
		additem(M_GPU_ACTIVITY_AUTO,((gpu_mode!=NULL)&&(gpu_mode->getivalue() == 2)) ? "(*)" : "( )"); 	//2 pref
		additem(M_GPU_ACTIVITY_NEVER,((gpu_mode!=NULL)&&(gpu_mode->getivalue() == 3)) ? "(*)" : "( )"); 	//3 never
	    }
	    srv->statedom.releaseptr(tmpstatedom);
	}
	Item* network_mode = tmpccstatusdom->findItem("network_mode");
	additem("",""); //delimiter
	additem(M_NET_ACTIVITY_ALWAYS, ((network_mode!=NULL)&&(network_mode->getivalue() == 1)) ? "(*)" : "( )"); //1 always
	additem(M_NET_ACTIVITY_AUTO,((network_mode!=NULL)&&(network_mode->getivalue() == 2)) ? "(*)" : "( )"); 	//2 pref
	additem(M_NET_ACTIVITY_NEVER,((network_mode!=NULL)&&(network_mode->getivalue() == 3)) ? "(*)" : "( )"); 	//3 never
	srv->ccstatusdom.releaseptr(tmpccstatusdom);
    }
    additem(NULL,NULL);
}


bool ActivitySubMenu::action()
{
    putevent(new NEvent(NEvent::evKB, KEY_F(9))); //закрыть все меню
    if (srv != NULL)
    {
	if ( strcmp(item_name(current_item(menu)),M_ACTIVITY_ALWAYS) == 0 )
	    srv->opactivity("always");
	if ( strcmp(item_name(current_item(menu)),M_ACTIVITY_AUTO) == 0 )
	    srv->opactivity("auto");
	if ( strcmp(item_name(current_item(menu)),M_ACTIVITY_NEVER) == 0 )
	    srv->opactivity("never");
	if ( strcmp(item_name(current_item(menu)),M_GPU_ACTIVITY_ALWAYS) == 0 )
	    srv->opgpuactivity("always");
	if ( strcmp(item_name(current_item(menu)),M_GPU_ACTIVITY_AUTO) == 0 )
	    srv->opgpuactivity("auto");
	if ( strcmp(item_name(current_item(menu)),M_GPU_ACTIVITY_NEVER) == 0 )
	    srv->opgpuactivity("never");
	if ( strcmp(item_name(current_item(menu)),M_NET_ACTIVITY_ALWAYS) == 0 )
	    srv->opnetactivity("always");
	if ( strcmp(item_name(current_item(menu)),M_NET_ACTIVITY_AUTO) == 0 )
	    srv->opnetactivity("auto");
	if ( strcmp(item_name(current_item(menu)),M_NET_ACTIVITY_NEVER) == 0 )
	    srv->opnetactivity("never");
    }
    return true;
}


//=============================================================================================


ProjectListSubMenu::ProjectListSubMenu(NRect rect, Srv* srv, std::string projname) : NMenu(rect)
{
    this->srv = srv;
    this->projname = projname;
    additem(M_UPDATE_PROJECT,"");
    additem(M_SUSPEND_PROJECT ,"");
    additem(M_RESUME_PROJECT,"");
    additem(M_NO_NEW_TASKS_PROJECT,"");
    additem(M_ALLOW_NEW_TASKS_PROJECT,"");
    additem(M_RESET_PROJECT,"");
    additem(M_DETACH_PROJECT,"");
    additem(NULL,NULL);
}


bool ProjectListSubMenu::action()
{
    if (srv != NULL)
    {
	const char* sop = NULL;
	bool confirmed = false;

	if ( strcmp(item_name(current_item(menu)),M_SUSPEND_PROJECT) == 0 )
	{//Suspend project
	    confirmed = true; //действие не требует подтверждения юзером
	    sop = "project_suspend";
	}
	if ( strcmp(item_name(current_item(menu)),M_RESUME_PROJECT) == 0 )
	{//Resume project
	    confirmed = true; //действие не требует подтверждения юзером
	    sop = "project_resume";
	}
	if ( strcmp(item_name(current_item(menu)),M_UPDATE_PROJECT) == 0 )
	{//Update project
	    confirmed = true; //действие не требует подтверждения юзером
	    sop = "project_update";
	}
	if ( strcmp(item_name(current_item(menu)),M_NO_NEW_TASKS_PROJECT) == 0 )
	{//No New Task project
	    confirmed = true; //действие не требует подтверждения юзером
	    sop = "project_nomorework";
	}
	if ( strcmp(item_name(current_item(menu)),M_ALLOW_NEW_TASKS_PROJECT) == 0 )
	{//Allow More Work project
	    confirmed = true; //действие не требует подтверждения юзером
	    sop = "project_allowmorework";
	}
	if ( strcmp(item_name(current_item(menu)),M_RESET_PROJECT) == 0 )
	{//Reset project
	    sop = "project_reset";
	}
	if ( strcmp(item_name(current_item(menu)),M_DETACH_PROJECT) == 0 )
	{//Detach project
	    sop = "project_detach";
	}
	if (sop)
	{
	    TuiEvent* ev = new TuiEvent(evPROJECTOP, srv, projname.c_str(),sop);
	    ev->bdata1 = confirmed;
	    putevent(ev);
	}
    }
    //создаем событие закрывающее меню
    putevent(new NEvent(NEvent::evKB, KEY_F(9)));
    return true;
}


void ProjectListSubMenu::eventhandle(NEvent* ev) 	//обработчик событий
{
    if ( ev->done )
	return;
    NMenu::eventhandle(ev); //предок
    if ( ev->done )
	return;
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case KEY_RIGHT: //блокируем стрелку вправо
		break;
	    case KEY_LEFT:
		putevent(new NEvent(NEvent::evKB, 27)); //закрыть это подменю
		break;
	    default:
		ev->done = false; //нет реакции на этот код
	} //switch
	if (ev->done) //если обработали, то нужно перерисоваться
	    refresh();
    }
}

//=============================================================================================


ProjectAllListSubMenu::ProjectAllListSubMenu(NRect rect, Srv* srv) : NMenu(rect)
{
    this->srv = srv;
    if (srv != NULL)
    {
	srv->updateallprojects();
	if ( srv->allprojectsdom != NULL)
	{
	    Item* projects = srv->allprojectsdom->findItem("projects");
	    if (projects != NULL)
	    {
		std::vector<Item*> projlist = projects->getItems("project");
		for (int i = 0; i < projlist.size(); i++)
		{
		    Item* name = projlist[i]->findItem("name");
		    Item* general_area = projlist[i]->findItem("general_area");
		    if (name != NULL)
		    {
			std::string status = ""; //строка тематика проекта
			if (general_area != NULL)
			    status = general_area->getsvalue();
			status.resize(20);
			additem(name->getsvalue(),status.c_str());
		    }
		}
	    }
	}
    }
    additem(NULL,NULL);
}


bool ProjectAllListSubMenu::action()
{
    if (srv != NULL)
    {
	const char* prjname = item_name(current_item(menu));
	//создаем подменю для выбора новый/существующий пользователь
	int begincol = getwidth() - 2; //смещение на экране по горизонтали
	int beginrow = 2 + item_index(current_item(menu)) - top_row(menu); //смещение на экране по вертикали
	insert(new ProjectUserExistSubMenu(NRect(5,25,beginrow, begincol), srv, prjname));
    }
    return true;
}


void ProjectAllListSubMenu::eventhandle(NEvent* ev) 	//обработчик событий
{
    if ( ev->done )
	return;
    NMenu::eventhandle(ev); //предок
    if ( ev->done )
	return;
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case KEY_RIGHT: //блокируем стрелку вправо
		break;
	    case KEY_LEFT:
		putevent(new NEvent(NEvent::evKB, 27)); //закрыть это подменю
		break;
	    case 27:
		if ( items.size() > 1 ) //1 из-за кроллбара
		    destroysubmenu();
		else
		    ev->done = false; //пусть обрабатывает владелец
		break;
	    default:
		ev->done = false; //нет реакции на этот код
	} //switch
	if (ev->done) //если обработали, то нужно перерисоваться
	    refresh();
    }
}


//=============================================================================================


ProjectAccMgrSubMenu::ProjectAccMgrSubMenu(NRect rect, Srv* srv) : NMenu(rect)
{
    this->srv = srv;
    //стандартные менеджеры
    if (srv != NULL)
    {
	srv->updateallprojects();
	if ( srv->allprojectsdom != NULL)
	{
	    Item* projects = srv->allprojectsdom->findItem("projects");
	    if (projects != NULL)
	    {
		std::vector<Item*> mgrlist = projects->getItems("account_manager");
		for (int i = 0; i < mgrlist.size(); i++)
		{
		    Item* name = mgrlist[i]->findItem("name");
		    if (name != NULL)
			additem(name->getsvalue(),"");
		}
	    }
	}
    }
    //менеджеры из конфига
    Item* boinctui_cfg = gCfg->getcfgptr();
    if (boinctui_cfg != NULL)
    {
	std::vector<Item*> mgrlist = boinctui_cfg->getItems("accmgr");
	std::vector<Item*>::iterator it;
	for (it = mgrlist.begin(); it != mgrlist.end(); it++)
	{
	    Item* name = (*it)->findItem("name");
	    if (name != NULL)
		additem(name->getsvalue(),"");
	}
    }
    //пункт добавить новый менеджер
    additem("","");
    additem(M_ADD_NEW_MGR,"");
    additem(NULL,NULL);
}


bool ProjectAccMgrSubMenu::action()
{
    putevent(new NEvent(NEvent::evKB, KEY_F(9))); //создаем событие закрывающее меню
    if (srv != NULL)
    {
	if (strcmp(item_name(current_item(menu)), M_ADD_NEW_MGR) != 0)
	    putevent(new TuiEvent(evADDACCMGR, srv, item_name(current_item(menu)))); //создаем событие открвыающее форму менеджера
	else
	    putevent(new TuiEvent(evADDACCMGR, srv, "Unnamed")); //создаем событие открвыающее форму менеджера
    }
    return true;
}


void ProjectAccMgrSubMenu::eventhandle(NEvent* ev) 	//обработчик событий
{
    if ( ev->done )
	return;
    NMenu::eventhandle(ev); //предок
    if ( ev->done )
	return;
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case KEY_RIGHT: //блокируем стрелку вправо
		break;
	    case KEY_LEFT:
		putevent(new NEvent(NEvent::evKB, 27)); //закрыть это подменю
		break;
	    default:
		ev->done = false; //нет реакции на этот код
	} //switch
	if (ev->done) //если обработали, то нужно перерисоваться
	    refresh();
    }
}

//=============================================================================================


ProjectUserExistSubMenu::ProjectUserExistSubMenu(NRect rect, Srv* srv, const char* prjname) : NMenu(rect)
{
    this->srv = srv;
    this->prjname = prjname;

    additem(M_PROJECT_USER_EXIST,"");
    additem(M_PROJECT_NEW_USER,"");
    additem(NULL,NULL);
}


bool ProjectUserExistSubMenu::action()
{
    //создаем событие закрывающее меню
    putevent(new NEvent(NEvent::evKB, KEY_F(9)));
    if (srv != NULL)
    {
	if ( strcmp(item_name(current_item(menu)),M_PROJECT_USER_EXIST) == 0 )
	    putevent(new TuiEvent(evADDPROJECT, srv, prjname.c_str(), true));
	if ( strcmp(item_name(current_item(menu)),M_PROJECT_NEW_USER) == 0 )
	    putevent(new TuiEvent(evADDPROJECT, srv, prjname.c_str(), false));
    }
    return true;
}


void ProjectUserExistSubMenu::eventhandle(NEvent* ev) 	//обработчик событий
{
    if ( ev->done )
	return;
    NMenu::eventhandle(ev); //предок
    if ( ev->done )
	return;
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case KEY_RIGHT: //блокируем стрелку вправо
		break;
	    case KEY_LEFT:
		putevent(new NEvent(NEvent::evKB, 27)); //закрыть это подменю
		break;
	    default:
		ev->done = false; //нет реакции на этот код
	} //switch
	if (ev->done) //если обработали, то нужно перерисоваться
	    refresh();
    }
}

