#include "postview.h"

PostView::PostView(QObject *parent) : QTextEdit(parent) {
	setReadOnly(true);
}
