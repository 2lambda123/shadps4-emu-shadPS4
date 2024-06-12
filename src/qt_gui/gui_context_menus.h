// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QDesktopServices>
#include <QHeaderView>
#include <QMenu>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "game_info.h"
#include "trophy_viewer.h"

class GuiContextMenus : public QObject {
    Q_OBJECT
public:
    void RequestGameMenu(const QPoint& pos, QVector<GameInfo> m_games, QTableWidget* widget,
                         bool isList) {
        QPoint global_pos = widget->viewport()->mapToGlobal(pos);
        int itemID = 0;
        if (isList) {
            itemID = widget->currentRow();
        } else {
            itemID = widget->currentRow() * widget->columnCount() + widget->currentColumn();
        }

        // Setup menu.
        QMenu menu(widget);
        QAction openFolder("Open Game Folder", widget);
        QAction openSfoViewer("SFO Viewer", widget);
        QAction openTrophyViewer("Trophy Viewer", widget);

        menu.addAction(&openFolder);
        menu.addAction(&openSfoViewer);
        menu.addAction(&openTrophyViewer);
        // Show menu.
        auto selected = menu.exec(global_pos);
        if (!selected) {
            return;
        }

        if (selected == &openFolder) {
            QString folderPath = QString::fromStdString(m_games[itemID].path);
            QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath));
        }

        if (selected == &openSfoViewer) {
            PSF psf;
            if (psf.open(m_games[itemID].path + "/sce_sys/param.sfo", {})) {
                int rows = psf.map_strings.size() + psf.map_integers.size();
                QTableWidget* tableWidget = new QTableWidget(rows, 2);
                tableWidget->setAttribute(Qt::WA_DeleteOnClose);
                connect(widget->parent(), &QWidget::destroyed, tableWidget,
                        [widget, tableWidget]() { tableWidget->deleteLater(); });

                tableWidget->verticalHeader()->setVisible(false); // Hide vertical header
                int row = 0;

                for (const auto& pair : psf.map_strings) {
                    QTableWidgetItem* keyItem =
                        new QTableWidgetItem(QString::fromStdString(pair.first));
                    QTableWidgetItem* valueItem =
                        new QTableWidgetItem(QString::fromStdString(pair.second));

                    tableWidget->setItem(row, 0, keyItem);
                    tableWidget->setItem(row, 1, valueItem);
                    keyItem->setFlags(keyItem->flags() & ~Qt::ItemIsEditable);
                    valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
                    row++;
                }
                for (const auto& pair : psf.map_integers) {
                    QTableWidgetItem* keyItem =
                        new QTableWidgetItem(QString::fromStdString(pair.first));
                    QTableWidgetItem* valueItem = new QTableWidgetItem(
                        QString("0x").append(QString::number(pair.second, 16)));

                    tableWidget->setItem(row, 0, keyItem);
                    tableWidget->setItem(row, 1, valueItem);
                    keyItem->setFlags(keyItem->flags() & ~Qt::ItemIsEditable);
                    valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
                    row++;
                }
                tableWidget->resizeColumnsToContents();
                tableWidget->resizeRowsToContents();

                int width = tableWidget->horizontalHeader()->sectionSize(0) +
                            tableWidget->horizontalHeader()->sectionSize(1) + 2;
                int height = (rows + 1) * (tableWidget->rowHeight(0));
                tableWidget->setFixedSize(width, height);
                tableWidget->sortItems(0, Qt::AscendingOrder);
                tableWidget->horizontalHeader()->setVisible(false);

                tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
                tableWidget->setWindowTitle("SFO Viewer");
                tableWidget->show();
            }
        }

        if (selected == &openTrophyViewer) {
            QString trophyPath = QString::fromStdString(m_games[itemID].serial);
            QString gameTrpPath = QString::fromStdString(m_games[itemID].path);
            TrophyViewer* trophyViewer = new TrophyViewer(trophyPath, gameTrpPath);
            trophyViewer->show();
            connect(widget->parent(), &QWidget::destroyed, trophyViewer,
                    [widget, trophyViewer]() { trophyViewer->deleteLater(); });
        }
    }

    int GetRowIndex(QTreeWidget* treeWidget, QTreeWidgetItem* item) {
        int row = 0;
        for (int i = 0; i < treeWidget->topLevelItemCount(); i++) { // check top level/parent items
            QTreeWidgetItem* currentItem = treeWidget->topLevelItem(i);
            if (currentItem == item) {
                return row;
            }
            row++;

            if (currentItem->childCount() > 0) { // check child items
                for (int j = 0; j < currentItem->childCount(); j++) {
                    QTreeWidgetItem* childItem = currentItem->child(j);
                    if (childItem == item) {
                        return row;
                    }
                    row++;
                }
            }
        }
        return -1;
    }

    void RequestGameMenuPKGViewer(
        const QPoint& pos, QStringList m_pkg_app_list, QTreeWidget* treeWidget,
        std::function<void(std::filesystem::path, int, int)> InstallDragDropPkg) {
        QPoint global_pos = treeWidget->viewport()->mapToGlobal(pos); // context menu position
        QTreeWidgetItem* currentItem = treeWidget->currentItem();     // current clicked item
        int itemIndex = GetRowIndex(treeWidget, currentItem);         // row

        QMenu menu(treeWidget);
        QAction installPackage("Install PKG", treeWidget);

        menu.addAction(&installPackage);

        auto selected = menu.exec(global_pos);
        if (!selected) {
            return;
        }

        if (selected == &installPackage) {
            QStringList pkg_app_ = m_pkg_app_list[itemIndex].split(";;");
            std::filesystem::path path(pkg_app_[9].toStdString());
#ifdef _WIN32
            path = std::filesystem::path(pkg_app_[9].toStdWString());
#endif
            InstallDragDropPkg(path, 1, 1);
        }
    }
};
