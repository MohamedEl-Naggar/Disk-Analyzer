#include <QApplication>
#include <QDesktopWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QFileIconProvider>
#include "file_analyzer.cpp"
using namespace std;

string size_conv(uintmax_t);
string remove_d(string);
void round1(string&);

int main(int argc, char *argv[])
{
    tree files_tree;
    files_tree = get_disk();
    node* files_ptr;
    files_ptr = files_tree.root;

    QApplication app(argc, argv);

    QTreeWidget *tree = new QTreeWidget;
    QStringList headers = {"Name", "Size"};

    tree->setHeaderLabels(headers);
    tree->setColumnCount(2);

    QFileIconProvider provider;
    QIcon folderIcon = provider.icon(QFileIconProvider::Folder);
    QIcon fileIcon = provider.icon(QFileIconProvider::File);
    QIcon myIcon;

    QTreeWidgetItem *root = new QTreeWidgetItem;
    QTreeWidgetItem *newItem, *oldItem;

    root->setText(0, QString(QString::fromStdString(files_ptr->name)));
    root->setText(1, QString(QString::fromStdString(size_conv(files_ptr->size))));
    root->setIcon(0, folderIcon);

    oldItem = root;
    int old_depth = -1, new_depth = 0;

    for(files_ptr = files_tree.dfs_iterator(); files_ptr != NULL; files_ptr = files_tree.dfs_iterator())
    {
        new_depth = files_ptr->depth;
        newItem = new QTreeWidgetItem;
        newItem->setText(0, QString(QString::fromStdString(files_ptr->name)));
        newItem->setText(1, QString(QString::fromStdString(size_conv(files_ptr->size))));
        myIcon = (files_ptr->is_directory)? folderIcon : fileIcon;
        newItem->setIcon(0, myIcon);

        for (int i = 0; i < old_depth - new_depth + 1; i++)
            oldItem = oldItem->parent();

        oldItem->addChild(newItem);
        
        old_depth = new_depth;
        oldItem = newItem;
    }

    const QSize availableSize = QApplication::desktop()->availableGeometry(tree).size();
    tree->addTopLevelItem(root);
    tree->setWindowTitle(QObject::tr("Tree View"));
    tree->setAnimated(false);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    tree->setAlternatingRowColors(true);
    tree->resize(availableSize / 2);
    tree->setColumnWidth(0, tree->width() / 3);
    tree->setWindowTitle(QObject::tr("Tree View"));
    tree->show();

    return app.exec();
}

string size_conv(uintmax_t s)
{
    if(s == 0)
        return "0 B";
    else if(s == 1000)
        return "1.0 KB";
    else if(s % 1000 == s)
        return remove_d(to_string(s).append(" B"));
    else if(s == 1000000)
        return "1.0 MB";
    else if(s % 1000000 == s)
        return remove_d(to_string(s/1000.0).append(" KB"));
    else if(s == 1000000000)
        return "1.0 GB";
    else if(s % 1000000000 == s)
        return remove_d(to_string(s/1000000.0).append(" MB"));
    else
        return remove_d(to_string(s/1000000000.0).append(" GB"));
}

string remove_d(string s)
{
    int length = s.length();
    char prefix = s[length - 2];
    string new_s;
    switch (prefix){
        case ' ':
        break;
        case 'K':
        new_s = s.substr(0, length - 7);
        round1(new_s);
        s = new_s.append(" KB");
        break;
        case 'M':
        new_s = s.substr(0, length - 7);
        round1(new_s);
        s = new_s.append(" MB");
        break;
        case 'G':
        new_s = s.substr(0, length - 7);
        round1(new_s);
        s = new_s.append(" GB");
        break;
    }
    return s;
}

void round1(string &s)
{
    int length = s.length();
    if (s[length - 1] >= 5)
        s[length - 2]++;
    for (int i = length - 2; i > 0; i--)
        if(s[i] == ':')
        {
            s[i] = '0';
            if(s[i - 1] != '.')
                s[i - 1]++;
            else
                s[i - 2]++;
        }
    if (s[0] == ':'){
        s[0] = '0';
        s.insert(s.begin(), '1');
    }
    s.pop_back();
}