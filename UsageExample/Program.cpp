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
	w.Show();

	wquery::TextBox tb(&w);
	tb.SetSize({ 300, 25 });
	tb.SetPosition({ 50,50 });
	tb.SetAnchor(wquery::AnchorSettings(true, true, true, false));

	wquery::Button b(&w);
	b.SetSize({ 150,25 });
	b.SetPosition({ 125,100 });
	b.SetAnchor(wquery::AnchorSettings(true, false, true, true));
	b.SetText("Button");

	wquery::End(wquery::MainLoopType::GET_MSG);
	return 0;
}