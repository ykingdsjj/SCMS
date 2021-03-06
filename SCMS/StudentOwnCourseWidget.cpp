#include "StudentOwnCourseWidget.h"

QString stu_3;
bool sm_3;

StudentOwnCourseWidget::StudentOwnCourseWidget(bool mode, QString student, QWidget* parent) :QWidget(parent) {
	ui.setupUi(this);
	stu_3 = student;
	sm_3 = mode;
	TableDisplay();
}


void StudentOwnCourseWidget::TableDisplay() {
	if (!sm_3) {
		NTableDisplay();
		NStatus();
	}
	else {
		JWTableDisplay();
		JWStatus();
	}
}

void StudentOwnCourseWidget::NTableDisplay() {
	CourseManage scm;
	scm.ReadStudentFile(sm_3, stu_3);
	set<Course>CourseSet = scm.CourseSet;
	ui.tableWidget->setRowCount(CourseSet.size());
	int CurrentRow = 0;
	for (set<Course>::iterator it = CourseSet.begin();it != CourseSet.end();it++, CurrentRow++) {
		ui.tableWidget->setItem(CurrentRow, 0, new QTableWidgetItem(QString("%1").arg(it->ID, 3, 10, QLatin1Char('0'))));
		ui.tableWidget->setItem(CurrentRow, 1, new QTableWidgetItem(it->Name));
		ui.tableWidget->setItem(CurrentRow, 2, new QTableWidgetItem(it->Teacher));
		ui.tableWidget->setItem(CurrentRow, 3, new QTableWidgetItem(it->Type));
		ui.tableWidget->setItem(CurrentRow, 4, new QTableWidgetItem(it->PersonalAssistant));
	}
	ui.tableWidget->resizeColumnsToContents();
}

void StudentOwnCourseWidget::JWTableDisplay() {
	CourseManage scm;
	scm.ReadStudentFile(sm_3, stu_3);
	set<Course>CourseSet = scm.CourseSet;
	ui.tableWidget->setColumnCount(10);
	ui.tableWidget->setRowCount(CourseSet.size());
	QStringList qsl;
	qsl << QString::fromLocal8Bit("课程编号") << QString::fromLocal8Bit("课程名称") << QString::fromLocal8Bit("性质") << QString::fromLocal8Bit("开课院系") << QString::fromLocal8Bit("学分") << QString::fromLocal8Bit("学时") << QString::fromLocal8Bit("校区") << QString::fromLocal8Bit("教师") << QString::fromLocal8Bit("上课时间及地点") << QString::fromLocal8Bit("个人助教");
	ui.tableWidget->setHorizontalHeaderLabels(qsl);
	int CurrentRow = 0;
	for (set<Course>::iterator it = CourseSet.begin();it != CourseSet.end();it++, CurrentRow++) {
		ui.tableWidget->setItem(CurrentRow, 0, new QTableWidgetItem(it->JWID));
		ui.tableWidget->setItem(CurrentRow, 1, new QTableWidgetItem(it->Name));
		ui.tableWidget->setItem(CurrentRow, 2, new QTableWidgetItem(it->Type));
		ui.tableWidget->setItem(CurrentRow, 3, new QTableWidgetItem(it->College));
		ui.tableWidget->setItem(CurrentRow, 4, new QTableWidgetItem(it->Score));
		ui.tableWidget->setItem(CurrentRow, 5, new QTableWidgetItem(it->Hour));
		ui.tableWidget->setItem(CurrentRow, 6, new QTableWidgetItem(it->Campus));
		ui.tableWidget->setItem(CurrentRow, 7, new QTableWidgetItem(it->Teacher));
		ui.tableWidget->setItem(CurrentRow, 8, new QTableWidgetItem(it->TPWQS));
		ui.tableWidget->setItem(CurrentRow, 9, new QTableWidgetItem(it->PersonalAssistant));
	}
	ui.tableWidget->resizeColumnsToContents();
}

void StudentOwnCourseWidget::NStatus() {
	ui.tableWidget_2->setRowCount(2);
	QStringList qsl;
	qsl << QString::fromLocal8Bit("专业课") << QString::fromLocal8Bit("非专业课");
	ui.tableWidget_2->setVerticalHeaderLabels(qsl);
	CourseManage scm;
	scm.ReadStudentFile(sm_3, stu_3);
	ui.tableWidget_2->setItem(0, 0, new QTableWidgetItem(QString::number(scm.CompulsoryCount())));
	ui.tableWidget_2->setItem(1, 0, new QTableWidgetItem(QString::number(scm.CourseSet.size() - scm.CompulsoryCount())));
	ui.tableWidget_2->resizeColumnsToContents();
}

void StudentOwnCourseWidget::JWStatus() {
	CourseManage scm;
	scm.ReadStudentFile(sm_3, stu_3);
	ui.tableWidget_2->setItem(0, 0, new QTableWidgetItem(QString::number(scm.TypeCount(1))));
	ui.tableWidget_2->setItem(1, 0, new QTableWidgetItem(QString::number(scm.TypeCount(2))));
	ui.tableWidget_2->setItem(2, 0, new QTableWidgetItem(QString::number(scm.TypeCount(3))));
	ui.tableWidget_2->setItem(3, 0, new QTableWidgetItem(QString::number(scm.TypeCount(4))));
	ui.tableWidget_2->setItem(4, 0, new QTableWidgetItem(QString::number(scm.TypeCount(5))));
	ui.tableWidget_2->setItem(5, 0, new QTableWidgetItem(QString::number(scm.TypeCount(6))));
	ui.tableWidget_2->resizeColumnsToContents();
}

void StudentOwnCourseWidget::DeleteCourse() {
	CourseManage scm;
	CourseManage cm;
	scm.ReadStudentFile(sm_3, stu_3);
	cm.ReadFile(sm_3);
	int n = ui.tableWidget->currentRow();
	Course course = scm.getNthCourse(n);
	scm.RemoveNth(n);
	cm.RemoveFromCourse(stu_3, course);
	cm.RemoveAssistant(stu_3, course);
	scm.WriteStudentFile(sm_3, stu_3);
	cm.WriteFile(sm_3);
	QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("退课成功"));
	if (!scm.CourseCheck() && !sm_3)QMessageBox::warning(this, QString::fromLocal8Bit("课程不足"), QString::fromLocal8Bit("请保证至少选择4门专业课与2门非专业课"));
	TableDisplay();
}

void StudentOwnCourseWidget::SelectAssistant() {
	CourseManage scm;
	scm.ReadStudentFile(sm_3, stu_3);
	SetAssistantDialog* sad = new SetAssistantDialog(sm_3, stu_3, scm.getNthCourse(ui.tableWidget->currentRow()), Q_NULLPTR);
	sad->show();
}

void StudentOwnCourseWidget::AssistantApply() {
	CourseManage scm;
	CourseManage cm;
	scm.ReadStudentFile(sm_3, stu_3);
	cm.ReadFile(sm_3);
	if (!cm.ApplyCheck(stu_3)) {
		QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("报名课程已满两门"));
	}
	else if (!cm.AddAssistant(stu_3, scm.getNthCourse(ui.tableWidget->currentRow()))) {
		QMessageBox::information(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请勿重复报名"));
	}
	else {
		cm.WriteFile(sm_3);
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("报名成功"));
	}
}

void StudentOwnCourseWidget::Exit() {
	this->close();
}

void StudentOwnCourseWidget::Refresh() {
	TableDisplay();
}

void StudentOwnCourseWidget::View(bool mode) {
	if (!sm_3) {
		QMessageBox::information(this, QString::fromLocal8Bit("操作失败"), QString::fromLocal8Bit("该模式下不可用"));
	}
	ViewScheduleWidget* vsw = new ViewScheduleWidget(mode, stu_3, Q_NULLPTR);
	vsw->show();
}

void StudentOwnCourseWidget::AddConnection() {
	if (!sm_3) {
		QMessageBox::information(this, QString::fromLocal8Bit("操作失败"), QString::fromLocal8Bit("该模式下不可用"));
	}
	else {
		AddConnectionWidget* acw = new AddConnectionWidget;
		acw->show();
	}
}

void StudentOwnCourseWidget::TimeTable() {
	View(0);
}

void StudentOwnCourseWidget::ViewSchedule() {
	View(1);
}