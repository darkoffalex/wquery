#include "../WQuery/Include/wquery/wquery.h"

int main(int argc, char* argv[])
{
	wquery::Begin();

	wquery::Window w;
	w.SetTitle("Sample form");
	w.SetSize({ 400, 200 }, true);
	w.SetMinSizes({ 400,200 }, true);
	w.SetPosition({ 100,100 });
	w.SetClosesProgram(true);
	w.SetIcon(wquery::GetExeDir().append("\\logo.ico"));
	w.Show();

	wquery::End(wquery::MainLoopType::GET_MSG);
	return 0;
}