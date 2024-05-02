#include "pch.h"
#include "file.h"

#include "main.h"
#include "Calendar.h"
#include "file.h"
#include "Cassette.h"
#include "Sheet.h"

#include "SQL.h"
#include "Exel.h"
#include "libxl.h"

//int logoId = book->addPicture(L"picture.jpg");
//sheet->setPicture(2, 1, logoId);
//book->dateUnpack(sheet->readNum(3, 1), &year, &month, &day, &hour, &min, &sec);


#define FSTART std::string(std::string("-->") + std::string(__FUNCTION__) + " ")
#define FSTOP std::string(std::string("<--") + std::string(__FUNCTION__) + " ")
#define FSTART std::string(std::string("-->") + std::string(__FUNCTION__) + " ")
#define TSTOP (std::string(__FUNCTION__) + " ")

#define CATCH(_s) catch(std::exception& exc){SendDebug(std::string((_s)) + exc.what());}catch(...){SendDebug(std::string((_s)) + "Unknown error");}


const char* NameKey = "ERICK RUBEN GONZALEZ BELTRAN";
const char* PartKey = "windows-202c280c0ac0ec046db46a60a9obmek6";
using namespace libxl;

std::vector<Format*> numFormat;


void CreateFormat(Book* book)
{
    Format* numFormat0 = book->addFormat();
    numFormat0->setNumFormat(NUMFORMAT_GENERAL);
    numFormat0->setAlignH(ALIGNH_CENTER);
    numFormat0->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat0->setBorder(BORDERSTYLE_THIN);

    int custom8 = book->addCustomNumFormat("[Magenta]");
    Format* numFormat8 = book->addFormat();
    numFormat8->setNumFormat(custom8);
    //numFormat0->setNumFormat(NUMFORMAT_GENERAL);
    numFormat8->setAlignH(ALIGNH_CENTER);
    numFormat8->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat8->setBorder(BORDERSTYLE_THIN);

    Format* numFormat1 = book->addFormat();
    numFormat1->setNumFormat(NUMFORMAT_NUMBER);
    numFormat1->setAlignH(ALIGNH_CENTER);
    numFormat1->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat1->setBorder(BORDERSTYLE_THIN);

    int custom0 = book->addCustomNumFormat("0.0");
    Format* numFormat2 = book->addFormat();
    numFormat2->setNumFormat(custom0);
    numFormat2->setAlignH(ALIGNH_CENTER);
    numFormat2->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat2->setBorder(BORDERSTYLE_THIN);

    int custom1 = book->addCustomNumFormat("[Magenta][<=0]0.0;[Blue]0.0");
    Format* numFormat3 = book->addFormat();
    numFormat3->setNumFormat(custom1);
    numFormat3->setAlignH(ALIGNH_CENTER);
    numFormat3->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat3->setBorder(BORDERSTYLE_THIN);


    Format* numFormat4 = book->addFormat();
    numFormat4->setNumFormat(custom1);
    numFormat4->setAlignH(ALIGNH_CENTER);
    numFormat4->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat4->setBorder(BORDERSTYLE_MEDIUM);

    Format* numFormat5 = book->addFormat();
    numFormat5->setNumFormat(book->addCustomNumFormat("000000"));
    numFormat5->setAlignH(ALIGNH_CENTER);
    numFormat5->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat5->setBorder(BORDERSTYLE_THIN);

    Format* numFormat6 = book->addFormat();
    numFormat6->setNumFormat(book->addCustomNumFormat("0000"));
    numFormat6->setAlignH(ALIGNH_CENTER);
    numFormat6->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat6->setBorder(BORDERSTYLE_THIN);

    Format* numFormat7 = book->addFormat();
    numFormat7->setNumFormat(book->addCustomNumFormat("000"));
    numFormat7->setAlignH(ALIGNH_CENTER);
    numFormat7->setAlignV(ALIGNV_DISTRIBUTED);
    numFormat7->setBorder(BORDERSTYLE_THIN);

    numFormat.push_back(numFormat0);
    numFormat.push_back(numFormat1);
    numFormat.push_back(numFormat2);
    numFormat.push_back(numFormat3);
    numFormat.push_back(numFormat4);
    numFormat.push_back(numFormat5);
    numFormat.push_back(numFormat6);
    numFormat.push_back(numFormat7);
    numFormat.push_back(numFormat8);
}


void ReleaseBook(Book** book)
{
    if(numFormat.size())
        numFormat.erase(numFormat.begin(), numFormat.end());

    if(*book)
    {
        (*book)->release();
        (*book) = NULL;
    }
}

//Чтение Exel файла
Book* LoadBook(std::string fname)
{
    //SendDebug(FSTART, fname);

    //g_lock.lock();
    //boost::shared_lock<boost::shared_mutex> lock(g_lock);
    Book* book = NULL;
    try
    {
        book = xlCreateXMLBook();

        if(!book) book = xlCreateBook();
        if(!book)
            throw std::exception("book == NULL");

        if(!book->load(fname.c_str()))
        {
            book->release();
            book = xlCreateBook();

            if(!book)
                throw std::exception("book == NULL");

            if(!book->load(fname.c_str()))
            {
                std::string s = book->errorMessage();
                throw std::exception((s + " fname: \"" + fname + "\" ").c_str());
            }
        }
        book->setKey(NameKey, PartKey);
        CreateFormat(book);
        //SendDebug(FSTOP, fname);
        return book;
    }
    catch(std::exception& exc)
    {
        SendDebug(__FUNCTION__, exc.what());
    }
    catch(...)
    {
        SendDebug(__FUNCTION__, "Unknown error");
    };

    ReleaseBook(&book);

    //SendDebug(FSTOP, fname);

    return NULL;

}


//Создание новой книги Эксела
Book* NewBook(std::string fname, int ID)
{
    //SendDebug(FSTART, fname);
    try
    {
        //std::vector<boost::filesystem::path> P = getDirAll(CurrentDirPatch);
        //for(auto p : P)
        //{
        //    if(p.generic_string() == fname)
        //    {
        //        SendDebug(FSTOP, "");
        //        return LoadBook(fname);
        //    }
        //}
        if(!LoadRessurse(fname, ID))
            throw std::exception(("Not LoadRessurse = \"" + fname + "\"" + " ID = " + std::to_string(ID)).c_str());
        return LoadBook(fname);
    }
    catch(std::exception& exc)
    {
        SendDebug(__FUNCTION__, exc.what());
    }
    catch(...)
    {
        SendDebug(__FUNCTION__, "Unknown error");
    };

    //SendDebug(FSTOP, fname);
    return NULL;
}

bool WriteRect(Sheet* sheet, int row, int col, std::string out)
{
    return sheet->writeStr(row, col, out.c_str());
}
bool WriteRect(Sheet* sheet, int row, int col, std::string out, Format* format)
{
    return sheet->writeStr(row, col, out.c_str(), format);
}
bool WriteRect(Sheet* sheet, int row, int col, float out, Format* format)
{
    return sheet->writeNum(row, col, out, format);
}
bool WriteRect(Sheet* sheet, int row, int col, double out, Format* format)
{
    return sheet->writeNum(row, col, out, format);
}
bool WriteRect(Sheet* sheet, int row, int col, int out, Format* format)
{
    return sheet->writeNum(row, col, out, format);
}

////Запись строки листа
void SaveRow(Sheet* sheet, int row, TSheet& St)
{
    float BB = 0;
    int AA = 0;

    //if(St.DataTime_End.length() > 0)WriteRect(sheet, row, cas::DataTime_End, St.DataTime_End, numFormat[0]);
    //if(St.DataTime_All.length() > 0)WriteRect(sheet, row, cas::DataTime_All, St.DataTime_All, numFormat[0]);

    //AA = atoi(St.Temper.c_str()); WriteRect(sheet, row, cas::Temper, AA, numFormat[2]);
    //AA = atoi(St.Speed.c_str()); WriteRect(sheet, row, cas::Speed, AA, numFormat[2]);

    //BB = (float)atof(St.Za_PT3.c_str()); WriteRect(sheet, row, cas::Za_PT3, BB, numFormat[2]);
    //BB = (float)atof(St.Za_TE3.c_str()); WriteRect(sheet, row, cas::Za_TE3, BB, numFormat[2]);

    //BB = (float)atof(St.LaminPressTop.c_str()); WriteRect(sheet, row, cas::LamPressTop, BB, numFormat[2]);
    //BB = (float)atof(St.LaminPressBot.c_str()); WriteRect(sheet, row, cas::LamPressBot, BB, numFormat[2]);
    //BB = (float)atof(St.PosClapanTop.c_str()); WriteRect(sheet, row, cas::PosClapanTop, BB, numFormat[2]);
    //BB = (float)atof(St.PosClapanBot.c_str()); WriteRect(sheet, row, cas::PosClapanBot, BB, numFormat[2]);
    //WriteRect(sheet, row, cas::Mask, St.Mask, numFormat[0]);

    //BB = (float)atof(St.Lam1PosClapanTop.c_str()); WriteRect(sheet, row, cas::Lam1PosClapanTop, BB, numFormat[2]);
    //BB = (float)atof(St.Lam1PosClapanBot.c_str()); WriteRect(sheet, row, cas::Lam1PosClapanBot, BB, numFormat[2]);
    //BB = (float)atof(St.Lam2PosClapanTop.c_str()); WriteRect(sheet, row, cas::Lam2PosClapanTop, BB, numFormat[2]);
    //BB = (float)atof(St.Lam2PosClapanBot.c_str()); WriteRect(sheet, row, cas::Lam2PosClapanBot, BB, numFormat[2]);

    //BB = (float)atof(St.LAM_TE1.c_str()); WriteRect(sheet, row, cas::Lam_TE1, BB, numFormat[2]);


    //AA = atoi(St.News.c_str()); WriteRect(sheet, row, cas::News, AA, numFormat[1]);

    //if(AA)
    //{
    //    BB = (float)atof(St.Top1.c_str()); WriteRect(sheet, row, cas::Top1, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Top2.c_str()); WriteRect(sheet, row, cas::Top2, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Top3.c_str()); WriteRect(sheet, row, cas::Top3, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Top4.c_str()); WriteRect(sheet, row, cas::Top4, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Top5.c_str()); WriteRect(sheet, row, cas::Top5, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Top6.c_str()); WriteRect(sheet, row, cas::Top6, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Top7.c_str()); WriteRect(sheet, row, cas::Top7, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Top8.c_str()); WriteRect(sheet, row, cas::Top8, BB, BB ? numFormat[4] : numFormat[3]);

    //    BB = (float)atof(St.Bot1.c_str()); WriteRect(sheet, row, cas::Bot1, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Bot2.c_str()); WriteRect(sheet, row, cas::Bot2, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Bot3.c_str()); WriteRect(sheet, row, cas::Bot3, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Bot4.c_str()); WriteRect(sheet, row, cas::Bot4, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Bot5.c_str()); WriteRect(sheet, row, cas::Bot5, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Bot6.c_str()); WriteRect(sheet, row, cas::Bot6, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Bot7.c_str()); WriteRect(sheet, row, cas::Bot7, BB, BB ? numFormat[4] : numFormat[3]);
    //    BB = (float)atof(St.Bot8.c_str()); WriteRect(sheet, row, cas::Bot8, BB, BB ? numFormat[4] : numFormat[3]);
    //    AA = atoi(St.Day.c_str()); WriteRect(sheet, row, cas::Day, AA, numFormat[1]);
    //    AA = atoi(St.Month.c_str()); WriteRect(sheet, row, cas::Month, AA, numFormat[1]);
    //    AA = atoi(St.Year.c_str()); WriteRect(sheet, row, cas::Year, AA, numFormat[1]);
    //    AA = atoi(St.CassetteNo.c_str()); WriteRect(sheet, row, cas::CassetteNo, AA, numFormat[1]);
    //    AA = atoi(St.SheetInCassette.c_str()); WriteRect(sheet, row, cas::SheetInCassette, AA, numFormat[1]);
    //}
}

bool AddSheet(Sheet* sheet, int row, TSheet& St)
{
 //SendDebug(FSTART, "");

 //SendDebug4("AddCassette", St);
 //WriteRect(sheet, row, cas::DataTime, St.Start_at, atoi(St.News.c_str()) ? numFormat[0] : numFormat[8]);

 //St.Start_at
    std::vector <std::string>split;
    boost::split(split, St.Start_at, boost::is_any_of(" "), boost::token_compress_on);
    if(split.size() == 2)
    {
        if(!WriteRect(sheet, row, cas::Data, split[0], numFormat[0])) return false;
        if(!WriteRect(sheet, row, cas::Time, split[1], numFormat[0])) return false;
    }
    if(!WriteRect(sheet, row, cas::Alloy, St.Alloy, numFormat[0])) return false;
    if(!WriteRect(sheet, row, cas::Thikness, St.Thikness, numFormat[0])) return false;
    if(!WriteRect(sheet, row, cas::Melt, St.Melt, numFormat[5])) return false;
    if(!WriteRect(sheet, row, cas::PartNo, St.PartNo, numFormat[6])) return false;
    if(!WriteRect(sheet, row, cas::Pack, St.Pack, numFormat[6])) return false;
    if(!WriteRect(sheet, row, cas::Sheet, St.Sheet + "/" + St.SubSheet, numFormat[7])) return false;
    if(!WriteRect(sheet, row, cas::Temper, St.Temper, numFormat[7])) return false;          //Заданная температура в печи закалки, °С
    if(!WriteRect(sheet, row, cas::FactTemp, St.Temperature, numFormat[7])) return false;   //Фактическая температура в печи закалки, °С

    if(!WriteRect(sheet, row, cas::TimeForPlateHeat, St.TimeForPlateHeat, numFormat[7])) return false;  //Задание Время окончания нагрева
    if(!WriteRect(sheet, row, cas::DataTime_All, St.DataTime_All, numFormat[7])) return false;          //Время закалки мин
    if(!WriteRect(sheet, row, cas::Speed, St.Speed, numFormat[7])) return false;                        //Скорость закалки
    if(!WriteRect(sheet, row, cas::Za_PT3, St.Za_PT3, numFormat[7])) return false;                      //Давление воды в баке бар
    if(!WriteRect(sheet, row, cas::Za_TE3, St.Za_TE3, numFormat[7])) return false;                      //Темперратура воды в баке




    //SaveRow(sheet, row, St);
    //SendDebug(FSTOP, "");
    return true;
}
void SaveSheetListXlcx(HWND hWnd, std::string fname, int ID)
{
    Book* book = NULL;
    try
    {
        book = NewBook(fname, ID);
        if(book)
        {
            //MessageBox(hWnd, fname.c_str(), "", 0);
            Sheet* sheet = book->getSheet(0);
            if(!sheet)
            {
                std::string s = book->errorMessage();
                throw std::exception((s + " fname: \"" + fname + "\" ").c_str());
            }
            int row = sheet->lastRow();
            for(auto St : AllSheet)
            {
                //Дата, время загрузки листа в закалочную печь
                // Марка стали
                // Толщина листа, мм
                // Плавка
                // Партия
                // Пачка
                // Номер листа
                // Заданная температура
                // Скорость выдачи, мм/с
                // Скоростная секция Давление воды в баке верх
                // Скоростная секция Давление воды в баке низ
                // Скоростная секция Температура воды в баке верх
                // Скоростная секция Температура воды в баке низ
                // Скоростная секция Давление в верхнем коллекторе
                // Скоростная секция Давление в нижнем коллекторе
                // Скоростная секция Положение клапана верх
                // Скоростная секция Положение клапана низ
                // Скоростная секция Маскирование
                // Ламинарная секия 1 Положение клапана верх
                // Ламинарная секия 1 Положение клапана низ
                // Ламинарная секия 2 Положение клапана верх
                // Ламинарная секия 2 Положение клапана низ
                // Температура воды в поддоне
                // Кантовка	
                // Отклонения от плоскостности листа До кантовки 1
                // Отклонения от плоскостности листа До кантовки 2
                // Отклонения от плоскостности листа До кантовки 3
                // Отклонения от плоскостности листа До кантовки 4
                // Отклонения от плоскостности листа До кантовки 5
                // Отклонения от плоскостности листа До кантовки 6
                // Отклонения от плоскостности листа До кантовки 7
                // Отклонения от плоскостности листа До кантовки 8
                // Отклонения от плоскостности листа После кантовки 1
                // Отклонения от плоскостности листа После кантовки 2
                // Отклонения от плоскостности листа После кантовки 3
                // Отклонения от плоскостности листа После кантовки 4
                // Отклонения от плоскостности листа После кантовки 5
                // Отклонения от плоскостности листа После кантовки 6
                // Отклонения от плоскостности листа После кантовки 7
                // Отклонения от плоскостности листа После кантовки 8
                // ID Листа День	
                // ID Листа Месяц	
                // ID Листа Год	
                // ID Листа Касета	
                // ID Листа Лист
                // Дата, время выдачи листа из закалочной печи
                // Продолжительность закалки, мин
            //sheet->insertRow(3, i++, true);
            //if(St.Start_at.length())
                if(!AddSheet(sheet, row++, St))
                {
                    std::string ss = book->errorMessage();
                    SendDebug("Exel", ss);
                }
            }


        //WriteRect(sheet, row, cas4::Close, a.Start_at, numFormat[0]);

            book->save(fname.c_str());
        }
    }
    catch(std::exception& exc)
    {
        SendDebug(__FUNCTION__, exc.what());
    }
    catch(...)
    {
        SendDebug(__FUNCTION__, "Unknown error");
    };

    if(book)ReleaseBook(&book);
}


void SaveCassetteListXlcx(HWND hWnd, std::string fname, int ID)
{

    Book* book = NewBook(fname, ID);
    if(book)
    {
        //MessageBox(hWnd, fname.c_str(), "", 0);

        Sheet* sheet = book->getSheet(0);
        if(!sheet)
        {
            std::string s = book->errorMessage();
            throw std::exception((s + " fname: \"" + fname + "\" ").c_str());
        }

        int row = 2;
        int AA = 0;
        float BB = 0;
        for(auto& Cl : AllCassette)
        {
            WriteRect(sheet, row, cas4::DataTime, Cl.Create_at, numFormat[0]);
            AA = atoi(Cl.Year.c_str()); WriteRect(sheet, row, cas4::Year, AA, numFormat[1]);
            AA = atoi(Cl.Month.c_str()); WriteRect(sheet, row, cas4::Month, AA, numFormat[1]);
            AA = atoi(Cl.Day.c_str()); WriteRect(sheet, row, cas4::Day, AA, numFormat[1]);
            AA = atoi(Cl.CassetteNo.c_str()); WriteRect(sheet, row, cas4::CassetteNo, AA, numFormat[1]);
            AA = atoi(Cl.SheetInCassette.c_str()); WriteRect(sheet, row, cas4::SheetInCassette, AA, numFormat[1]);

            WriteRect(sheet, row, cas4::Close, Cl.Close_at, numFormat[0]);
            WriteRect(sheet, row, cas4::Run, Cl.Run_at, numFormat[0]);
            WriteRect(sheet, row, cas4::Error, Cl.Error_at, numFormat[0]);
            WriteRect(sheet, row, cas4::End, Cl.End_at, numFormat[0]);
            WriteRect(sheet, row, cas4::Delete, Cl.Delete_at, numFormat[0]);

            AA = atoi(Cl.Peth.c_str()); WriteRect(sheet, row, cas4::Peth, AA, numFormat[1]);
            //BB = (float)atof(Cl.TempRef.c_str()); WriteRect(sheet, row, cas4::TempRef, BB, numFormat[2]);         //Заданное значение температуры
            BB = (float)atof(Cl.PointTime_1.c_str()); WriteRect(sheet, row, cas4::PointTime_1, BB, numFormat[2]);   //Время разгона
            BB = (float)atof(Cl.PointRef_1.c_str()); WriteRect(sheet, row, cas4::PointRef_1, BB, numFormat[2]);     //Уставка температуры
            BB = (float)atof(Cl.TimeProcSet.c_str()); WriteRect(sheet, row, cas4::TimeProcSet, BB, numFormat[2]);   //Полное время процесса (уставка), мин
            BB = (float)atof(Cl.PointDTime_2.c_str()); WriteRect(sheet, row, cas4::PointDTime_2, BB, numFormat[2]); //Время выдержки


            row++;

        }
        book->save(fname.c_str());
    }
    ReleaseBook(&book);
}

extern HWND ListSheetTemp;
extern HWND ListCasseteTemp;

void SaveCassetteTemperPasport(HWND hWnd, Sheet* sheet)
{
    char ss[256];
    int iItemServ = (int)ListView_GetItemCount(ListCasseteTemp);
    HWND hWndHdr = (HWND)::SendMessage(ListCasseteTemp, LVM_GETHEADER, 0, 0);
    int count = (int)::SendMessage(hWndHdr, HDM_GETITEMCOUNT, 0, 0L) - 1;

    for(int l=0; l < iItemServ; l++)
    {

        for(int i = 0; i < count; i++)
        {
            ListView_GetItemText(ListCasseteTemp, l, i + 1, ss, 255);
            if(i)
            {
                //std::string sd = ss;
                //boost::replace_all(sd, ".", ",");
                WriteRect(sheet, l + 1, i, atof(ss), numFormat[2]);
            }
            else
                WriteRect(sheet, l + 1, i, ss, numFormat[0]);
        }
    }
}

void SaveSheetTemperPasport(HWND hWnd, Sheet* sheet)
{
    char ss[256];
    int iItemServ = (int)ListView_GetItemCount(ListSheetTemp);
    HWND hWndHdr = (HWND)::SendMessage(ListSheetTemp, LVM_GETHEADER, 0, 0);
    int count = (int)::SendMessage(hWndHdr, HDM_GETITEMCOUNT, 0, 0L) - 1;

    for(int l=0; l < iItemServ; l++)
    {
        for(int i = 0; i < count; i++)
        {
            ListView_GetItemText(ListSheetTemp, l, i+1, ss, 255);
            if(i)
            {
                //std::string sd = ss;
                //boost::replace_all(sd, ".", ",");
                WriteRect(sheet, l + 1, i, atof(ss), numFormat[2]);
                //WriteRect(sheet, l + 1, i, sd, numFormat[2]);
            }
            else
                WriteRect(sheet, l + 1, i, ss, numFormat[0]);
        }
    }
}

void SavePasport(HWND hWnd, Sheet* sheet)
{
    char ss[256];
    GetWindowText(GetDlgItem(hWnd, IDC_pAlloy), ss, 255);
    WriteRect(sheet, 3, 3, ss, NULL);   //Марка стали
    GetWindowText(GetDlgItem(hWnd, IDC_pThikness), ss, 255);
    WriteRect(sheet, 3, 9, ss, NULL);//Размер листа


    //Закалка
    //Дата и время загрузки
    GetWindowText(GetDlgItem(hWnd, IDC_pSheetDate), ss, 255); //Дата
    WriteRect(sheet, 8, 17, ss, NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_pSheetTime), ss, 255); //Время
    WriteRect(sheet, 8, 19, ss, NULL);

    //Время нахождения листа в закалочной печи. мин
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT11), ss, 255); //Задание
    WriteRect(sheet, 12, 17, atof(ss), NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT12), ss, 255); //Факт
    WriteRect(sheet, 12, 19, atof(ss), NULL);

    //Температура закалки, °С
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT13), ss, 255); //Задание
    WriteRect(sheet, 14, 17, atof(ss), NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT14), ss, 255); //Факт
    WriteRect(sheet, 14, 19, atof(ss), NULL);

    //Давление воды в коллекторе закал. машины, бар
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT15), ss, 255); //Задание
    WriteRect(sheet, 16, 17, atof(ss), NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT16), ss, 255); //Факт
    WriteRect(sheet, 16, 19, atof(ss), NULL);

    //Температура закалочной жидкости, °С
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT17), ss, 255); //Факт
    WriteRect(sheet, 18, 19, atof(ss), NULL);



    //Печи отпуска
    //Дата и время загрузки
    GetWindowText(GetDlgItem(hWnd, IDC_pCassetteDate), ss, 255); //Дата
    WriteRect(sheet, 22, 17, ss, NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_pCasseteTime), ss, 255); //Время
    WriteRect(sheet, 22, 19, ss, NULL);

    //Время нагрева до температуры отпуска, мин
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT18), ss, 255); //Задание
    WriteRect(sheet, 26, 17, atof(ss), NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT19), ss, 255); //Факт
    WriteRect(sheet, 26, 19, atof(ss), NULL);

    //Время выдержки при заданной температуре, мин
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT20), ss, 255); //Задание
    WriteRect(sheet, 28, 17, atof(ss), NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT21), ss, 255); //Факт
    WriteRect(sheet, 28, 19, atof(ss), NULL);

    //Температура отпуска, °С
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT22), ss, 255); //Задание
    WriteRect(sheet, 30, 17, atof(ss), NULL);
    GetWindowText(GetDlgItem(hWnd, IDC_EDIT23), ss, 255); //Факт
    WriteRect(sheet, 30, 19, atof(ss), NULL);
}


bool SavePasportExel(HWND hWnd)
{
    char sMelt[256];
    char sPartNo[256];
    char sPack[256];
    char sSheet[256];
    char sSlab[256];
    char ss[256];

    GetWindowText(GetDlgItem(hWnd, IDC_pMelt), sMelt, 255);
    GetWindowText(GetDlgItem(hWnd, IDC_pPartNo), sPartNo, 255);
    GetWindowText(GetDlgItem(hWnd, IDC_pPack), sPack, 255);
    GetWindowText(GetDlgItem(hWnd, IDC_pSheet), sSheet, 255);
    GetWindowText(GetDlgItem(hWnd, IDC_pSlab), sSlab, 255);

    sprintf_s(ss, 255, "%06u-%04u-%03u-%03u-%06u.xlsx",atoi(sMelt), atoi(sPartNo), atoi(sPack), atoi(sSheet),atoi(sSlab));
    std::string fname = ss;

    if(!CmdFileSaveXlsx(hWnd, ss))
        return FALSE;

    Book* book = NewBook(fname, IDR_DAT5);
    if(book)
    {
        Sheet* sheet = book->getSheet(0);
        if(!sheet)
        {
            std::string s = book->errorMessage();
            throw std::exception((s + " fname: \"" + fname + "\" ").c_str());
        }
        WriteRect(sheet, 5, 3, sMelt, NULL);    //Плавка
        WriteRect(sheet, 5, 7, sPartNo, NULL);  //Партия
        WriteRect(sheet, 5, 11, sPack, NULL);   //Пачка
        WriteRect(sheet, 5, 15, sSheet, NULL);  //Номер листа
        WriteRect(sheet, 5, 19, sSlab, NULL);   //Номер сляба

        SavePasport(hWnd, sheet);

        sheet = book->getSheet(1);
        if(!sheet)
        {
            std::string s = book->errorMessage();
            throw std::exception((s + " fname: \"" + fname + "\" ").c_str());
        }
        SaveSheetTemperPasport(hWnd, sheet);

        sheet = book->getSheet(2);
        if(!sheet)
        {
            std::string s = book->errorMessage();
            throw std::exception((s + " fname: \"" + fname + "\" ").c_str());
        }
        SaveCassetteTemperPasport(hWnd, sheet);

        book->save(fname.c_str());
    }
    ReleaseBook(&book);
    return TRUE;
}
