#include "akusearchwidget.h"

akuSearchWidget::akuSearchWidget(QTreeWidget *treeWidget, QWidget *parent) : QWidget(parent)
{
  closeAction = new KAction(this);
  closeAction -> setText(i18n("Find"));
  closeAction -> setCheckable(true);
  closeAction -> setIcon(KIcon("edit-find"));
  closeTool = new QToolButton(this);
  closeTool -> setIcon(KIcon("window-close"));
  closeTool ->setAutoRaise(true);
  QLabel *label = new QLabel(i18n("Find:"), this);
  searchLine = new akuSearchLine(treeWidget, this);
  QGridLayout *searchLayout = new QGridLayout(this);
  searchLayout -> addWidget(closeTool,1,1);
  searchLayout -> addWidget(label, 1,2);
  searchLayout -> addWidget(searchLine,1,3);
  QActionGroup *optionGroup = new QActionGroup(this);
  KAction *selectionFilter = new KAction(i18n("Selection Filter"), this);
  KAction *exclusionFilter = new KAction(i18n("Exclusion Filter"), this);
  exclusionFilter -> setCheckable(true);
  selectionFilter -> setCheckable(true);
  exclusionFilter -> setChecked(true);
  optionGroup -> addAction(exclusionFilter);
  optionGroup -> addAction(selectionFilter);
  QToolButton *optionButton = new QToolButton(this);
  optionButton -> setText(i18n("Options"));
  KMenu *optionMenu = new KMenu(i18n("Options"), this);
  optionMenu -> addAction(selectionFilter);
  optionMenu -> addAction(exclusionFilter);
  searchLayout -> addWidget(optionButton,1,4);
  KAction *caseSensitive = new KAction(i18n("Case Sensitive"), this);
  caseSensitive -> setCheckable(true);
  optionMenu -> addSeparator();
  optionMenu -> addAction(caseSensitive);
  optionButton -> setMenu(optionMenu);
  optionButton -> setPopupMode(QToolButton::MenuButtonPopup);
  connect(selectionFilter, SIGNAL(toggled(bool)), searchLine, SLOT(setFilterBySelection(bool)));
  connect(exclusionFilter, SIGNAL(toggled(bool)), searchLine, SLOT(setFilterByExclusion(bool)));
  connect(caseSensitive, SIGNAL(toggled(bool)), searchLine, SLOT(setFilterCaseSensitivity(bool)));
  connect(closeAction, SIGNAL(toggled(bool)), this, SLOT(handleCloseAction(bool)));
  connect(closeTool, SIGNAL(clicked()), this, SLOT(hideMe()));
  connect(optionButton, SIGNAL(clicked()), optionButton, SLOT(showMenu()));
}

akuSearchWidget::~akuSearchWidget()
{
}

akuSearchLine* akuSearchWidget::searchLineEdit()
{
  return searchLine;
}

void akuSearchWidget::hideMe()
{
  closeAction -> setChecked(false);
}

void akuSearchWidget::handleCloseAction(bool hide)
{
  setVisible(hide);
  if(hide)  searchLine -> setFocus( Qt::OtherFocusReason );
}

KAction* akuSearchWidget::getCloseAction()
{
  return closeAction;
}

QSize akuSearchWidget::sizeHint() const
{
 return QSize(500, 37);
}
