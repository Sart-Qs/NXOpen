#include <uf.h>
#include <uf_modl.h>
#include <uf_curve.h>
#include <uf_csys.h>
#include <uf_mtx.h>
#include <stdio.h>
#include <uf_ui.h>
#include <fstream>
char msg[120];
char t_a[] = "0.0";
char a[] = "0.0";
char b[] = "5.0";
char c[] = "5.0";
void ufusr(char* param, int* retcode, int paramLen)
{

	if (!UF_initialize())
	{
		tag_t objarray[4]; //тег для двух линий и двух дуг

		UF_FEATURE_SIGN sign = UF_NULLSIGN;
		tag_t lines[] = { 0,1,2,3,4,5,6 }; // Идентификатор объекта «линия»
		UF_CURVE_line_t line[7];

		uf_list_p_t list, //Список объектов, подлежащих выдавливанию (элементы сечения)
			features; //Список созданных идентификаторов объектов
		UF_MODL_create_list(&list);//создание пустого перечня объектов
		char* taper_angle = t_a;//Конусность
		char* limit[2] = { a, b };//Начало и конец выдавливания
		double ref_pt[4];//параметр не используется
		double dir[3] = { 0.0, 1.0, 0.0 };//Вектор выдавливания
		UF_FEATURE_SIGN create = UF_NULLSIGN;

		uf_list_p_t list1, //Список объектов, подлежащих выдавливанию (элементы сечения)
			features1; //Список созданных идентификаторов объектов
		UF_MODL_create_list(&list1);//создание пустого перечня объектов
		double ref_pt1[3];
		double dir1[3] = { -1.0, 0.0, 0.0 };//Вектор выдавливания
		UF_FEATURE_SIGN create1 = UF_POSITIVE;

		line[0].start_point[0] = 0.0; // X1
		line[0].start_point[1] = 0.0; // Y1
		line[0].start_point[2] = 0.0; // Z1
		line[0].end_point[0] = 0.0; // X2
		line[0].end_point[1] = 0.0; // Y2
		line[0].end_point[2] = 25.0; // Z2

		line[1].start_point[0] = 0.0; // X1
		line[1].start_point[1] = 0.0; // Y1
		line[1].start_point[2] = 25.0; // Z1
		line[1].end_point[0] = 160.0; // X2
		line[1].end_point[1] = 0.0; // Y2
		line[1].end_point[2] = 25.0; // Z2

		line[2].start_point[0] = 160.0; // X1
		line[2].start_point[1] = 0.0; // Y1
		line[2].start_point[2] = 25.0; // Z1
		line[2].end_point[0] = 160.0; // X2
		line[2].end_point[1] = 0.0; // Y2
		line[2].end_point[2] = 0.0; // Z2

		line[3].start_point[0] = 160.0; // X1
		line[3].start_point[1] = 0.0; // Y1
		line[3].start_point[2] = 0.0; // Z1
		line[3].end_point[0] = 0.0; // X2
		line[3].end_point[1] = 0.0; // Y2
		line[3].end_point[2] = 0.0; // Z2

		line[4].start_point[0] = 160.0; // X1
		line[4].start_point[1] = 0.0; // Y1
		line[4].start_point[2] = 25.0; // Z1
		line[4].end_point[0] = 160.0; // X2
		line[4].end_point[1] = 50.0; // Y2
		line[4].end_point[2] = 25.0; // Z2

		line[5].start_point[0] = 160.0; // X1
		line[5].start_point[1] = 50.0; // Y1
		line[5].start_point[2] = 25.0; // Z1
		line[5].end_point[0] = 160.0; // X2
		line[5].end_point[1] = 50.0; // Y2
		line[5].end_point[2] = 0.0; // Z2

		line[6].start_point[0] = 160.0; // X1
		line[6].start_point[1] = 50.0; // Y1
		line[6].start_point[2] = 0.0; // Z1
		line[6].end_point[0] = 160.0; // X2
		line[6].end_point[1] = 0.0; // Y2
		line[6].end_point[2] = 0.0; // Z2


		tag_t arc_id, wcs_tag; //тэги окружности
		//и мировой системы координат 
		UF_CURVE_arc_t arc_coords; //структура свойств дуги
		// объявление данных для матрицы поворота
		//зададим угол 45° и пересчитаем его в радианы
		double ugol_Y = 45 * DEGRA;
		// объявим вектора осей Х и У
		double vec_X[3] = { 1, 0, 0 };

		//объявим матрицы поворотов:
		double mtxP[9], //вокруг оси Х
			mt[9]; //суммарную матрицу поворота

		//объявим теги систем координат
		tag_t teg_wcs, csys_id;
		// объявим точку начала координат
		double center[3] = { 0,0,0 };

		// начало кода работы с матрицами
		// получим матрицу поворота вокруг оси Х
		UF_MTX3_rotate_about_axis(vec_X, ugol_Y, mtxP);
		// перемножением получим суммарную матрицу поворота по обоим углам
		UF_MTX3_multiply(mtxP, mtxP, mt);
		// создадим тег суммарной матрицы поворота
		UF_CSYS_create_matrix(mt, &teg_wcs);
		// создадим тег системы координат на базе полученной матрицы
		UF_CSYS_create_csys(center, teg_wcs, &csys_id);
		// установим на экране созданную рабочую систему координат
		UF_CSYS_set_wcs(csys_id);
		// начало кода построения окружности
		arc_coords.start_angle = 90.0 * DEGRA; //начальный угол окружности
		arc_coords.end_angle = 270 * DEGRA; //конечный угол
		arc_coords.arc_center[0] = 15.0; //координата центра Х
		arc_coords.arc_center[1] = -12.5; //координата центра Y 
		arc_coords.arc_center[2] = 0.0; //координата центра Z 
		arc_coords.radius = 4.5; //радиус окружности
		UF_CSYS_ask_wcs(&wcs_tag); //получение абсолютных координат
		//"перенос" абсолютных координат на создаваемую окружность
		UF_CSYS_ask_matrix_of_object(wcs_tag, &arc_coords.matrix_tag);

		tag_t lineForNEG[] = {0,1};
		UF_CURVE_line_t lForNEG[2];

		lForNEG[0].start_point[0] = 15.0; // X1
		lForNEG[0].start_point[1] = 0.0; // Y1
		lForNEG[0].start_point[2] = 17.0; // Z1
		lForNEG[0].end_point[0] = 140.0; // X2
		lForNEG[0].end_point[1] = 0.0; // Y2
		lForNEG[0].end_point[2] = 17.0; // Z2

		lForNEG[1].start_point[0] = 15.0; // X1
		lForNEG[1].start_point[1] = 0.0; // Y1
		lForNEG[1].start_point[2] = 8.0; // Z1
		lForNEG[1].end_point[0] = 140.0; // X2
		lForNEG[1].end_point[1] = 0.0; // Y2
		lForNEG[1].end_point[2] = 8.0; // Z2


		tag_t arc_id1, wcs_tag1; //тэги окружности
		//и мировой системы координат 
		UF_CURVE_arc_t arc_coords1; //структура свойств дуги
		// объявление данных для матрицы поворота
		//зададим угол 45° и пересчитаем его в радианы
		double ugol_Y1 = 45 * DEGRA;
		// объявим вектора осей Х и У
		double vec_X1[3] = { 1, 0, 0 };

		//объявим матрицы поворотов:
		double mtxP1[9], //вокруг оси Х
			mt1[9]; //суммарную матрицу поворота

		//объявим теги систем координат
		tag_t teg_wcs1, csys_id1;
		// объявим точку начала координат
		double center1[3] = { 0,0,0 };
		// начало кода работы с матрицами
		// получим матрицу поворота вокруг оси Х
		UF_MTX3_rotate_about_axis(vec_X1, ugol_Y1, mtxP1);
		// перемножением получим суммарную матрицу поворота по обоим углам
		UF_MTX3_multiply(mtxP1, mtxP1, mt1);
		// создадим тег суммарной матрицы поворота
		UF_CSYS_create_matrix(mt1, &teg_wcs1);
		// создадим тег системы координат на базе полученной матрицы
		UF_CSYS_create_csys(center1, teg_wcs1, &csys_id1);
		// установим на экране созданную рабочую систему координат
		UF_CSYS_set_wcs(csys_id1);
		// начало кода построения окружности
		arc_coords1.start_angle = -90 * DEGRA; //начальный угол окружности
		arc_coords1.end_angle = 90 * DEGRA; //конечный угол
		arc_coords1.arc_center[0] = 140.0; //координата центра Х
		arc_coords1.arc_center[1] = -12.5; //координата центра Y 
		arc_coords1.arc_center[2] = 0.0; //координата центра Z 
		arc_coords1.radius = 4.5; //радиус окружности
		UF_CSYS_ask_wcs(&wcs_tag1); //получение абсолютных координат
		//"перенос" абсолютных координат на создаваемую окружность
		UF_CSYS_ask_matrix_of_object(wcs_tag1, &arc_coords1.matrix_tag);;

		uf_list_p_t list2, //Список объектов, подлежащих выдавливанию (элементы сечения)
			features2; //Список созданных идентификаторов объектов
		double ref_pt2[3];
		double dir2[3] = { 0.0, 1.0, 0.0 };//Вектор выдавливания
		UF_FEATURE_SIGN create2 = UF_NEGATIVE;
		char* limit2[2] = { a, c };

		for (int i = 0; i < 4; i++)
		{
			UF_CURVE_create_line(&line[i], &lines[i]);
			UF_MODL_put_list_item(list, lines[i]);
		}
		UF_MODL_create_extruded(list, taper_angle, limit, ref_pt, dir, create, &features);
		UF_MODL_put_list_item(list1, lines[2]);
		for (int i = 4; i < 7; i++)
		{
			UF_CURVE_create_line(&line[i], &lines[i]);
			UF_MODL_put_list_item(list1, lines[i]);
		}
		UF_MODL_create_extruded(list1, taper_angle, limit, ref_pt1, dir1, create1, &features1);

		UF_CURVE_create_line(&lForNEG[0], &objarray[0]);
		UF_CURVE_create_line(&lForNEG[1], &objarray[1]);
		UF_CURVE_create_arc(&arc_coords, &objarray[2]);
		UF_CURVE_create_arc(&arc_coords1, &objarray[3]);
		UF_MODL_create_list(&list2);//создание пустого перечня объектов

		for (int i = 0; i < 4; i++)
		{
			UF_MODL_put_list_item(list2, objarray[i]);
		}
		UF_MODL_create_extruded(list2, taper_angle, limit2, ref_pt2, dir2, create2, &features2);
		
	}

	UF_terminate();
}

//типовая функция завершения приложения
int ufusr_ask_unload(void) 
{
	return (UF_UNLOAD_IMMEDIATELY);
}

