#include <uf.h>
#include <uf_modl.h>
#include <uf_curve.h>


char t_a[] = "0.0";
char a[] = "0.0";
char b[] = "-4.0";
void ufusr(char* param, int* retcode, int paramLen)
{
	// определим создание нового самостоятельного тела 
	UF_FEATURE_SIGN sign = UF_NULLSIGN;
	//это тело должно начинаться в заданных координатах Х, У, Z 
	double cyl_orig[3] = { 0.0,0.0,0.0 };
	char height[125] = "23"; //высоту цилиндра зададим в 70 мм 
	char diam[125] = "8"; //диаметр-в 50 мм 
	//и орты направления главной диагонали координат будут по 1 
	double direction[3] = { 1.0, 0.0, 0.0 };
	tag_t cyl_obj; //определим переменную для будущего тега цилиндра

	//это тело должно начинаться в заданных координатах Х, У, Z 
	double cyl_orig1[3] = { 23.0,0.0,0.0 };
	char height1[125] = "5"; //высоту цилиндра зададим в 70 мм 
	char diam1[125] = "13"; //диаметр-в 50 мм 
	//и орты направления главной диагонали координат будут по 1 
	double direction1[3] = { 1.0, 0.0, 0.0 };
	tag_t cyl_obj1; //определим переменную для будущего тега цилиндра+

	

	tag_t lines[4] = { 0,1,2,3 }; // Идентификатор объекта «линия»
	UF_CURVE_line_t line[4];
	line[0].start_point[0] = 28.0; // X1
	line[0].start_point[1] = 1.5; // Y1
	line[0].start_point[2] = 7.0; // Z1
	line[0].end_point[0] = 28.0; // X2
	line[0].end_point[1] = 1.5; // Y2
	line[0].end_point[2] = -7.0; // Z2

	line[1].start_point[0] = 28.0; // X1
	line[1].start_point[1] = 1.5; // Y1
	line[1].start_point[2] = -7.0; // Z1
	line[1].end_point[0] = 28.0; // X2
	line[1].end_point[1] = -1.5; // Y2
	line[1].end_point[2] = -7.0; // Z2

	line[2].start_point[0] = 28.0; // X1
	line[2].start_point[1] = -1.5; // Y1
	line[2].start_point[2] = -7.0; // Z1
	line[2].end_point[0] = 28.0; // X2
	line[2].end_point[1] = -1.5; // Y2
	line[2].end_point[2] = 7.0; // Z2

	line[3].start_point[0] = 28.0; // X1
	line[3].start_point[1] = -1.5; // Y1
	line[3].start_point[2] = 7.0; // Z1
	line[3].end_point[0] = 28.0; // X2
	line[3].end_point[1] = 1.5; // Y2
	line[3].end_point[2] = 7.0; // Z2

	uf_list_p_t list, //Список объектов, подлежащих выдавливанию (элементы сечения)
		features; //Список созданных идентификаторов объектов
	UF_MODL_create_list(&list);//создание пустого перечня объектов
	char* taper_angle = t_a;//Конусность
	char* limit[2] = { a, b };//Начало и конец выдавливания
	double ref_pt[4];//параметр не используется
	double dir[3] = { 1.0, 0.0, 0.0 };//Вектор выдавливания
	UF_FEATURE_SIGN create = UF_NEGATIVE;

	for (int i = 0; i < 4; i++)
	{
		UF_CURVE_create_line(&line[i], &lines[i]);
		UF_MODL_put_list_item(list, lines[i]);
	}

	if (!UF_initialize())
	{
		//выполним построение цилиндра 
		UF_MODL_create_cyl1(sign, cyl_orig, height, diam, direction, &cyl_obj);
		UF_MODL_create_cyl1(sign, cyl_orig1, height1, diam1, direction1, &cyl_obj1);
		UF_MODL_create_extruded(list, taper_angle, limit, ref_pt, dir, create, &features);
	}
}

//типовая функция завершения приложения
int ufusr_ask_unload(void) 
{
	return (UF_UNLOAD_IMMEDIATELY);
}

