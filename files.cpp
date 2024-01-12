#include "files.h"
#include <QFileDialog>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>

FilesHandler::FilesHandler()
{
}

void FilesHandler::saveMap(Map *map)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save CSV File", "", "CSV Files (*.csv)");

    if (!fileName.isEmpty())
    {
        QFileInfo fileInfo(fileName);
        if (fileInfo.suffix().isEmpty())
        {
            // If the file name does not have an extension, append ".csv"
            fileName += ".csv";
        }

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            for (int x = 0; x < map->getNbColumns(); x++)
            {
                for (int y = 0; y < map->getNbRows(); y++)
                {
                    TileType tile_type = map->getTileType(x, y);
                    if (tile_type != TileType::Solid && tile_type != TileType::Start && tile_type != TileType::Target)
                    {
                        tile_type = TileType::Empty;
                    }
                    out << static_cast<uint8_t>(tile_type) << ",";
                }
                out << "\n";
            }

            file.close();
            qDebug() << "Matrix saved to CSV file:" << fileName;
        }
        else
        {
            qDebug() << "Unable to open file for writing:" << fileName;
        }
    }
}

void FilesHandler::loadMap(Map *map)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open CSV File", "", "CSV Files (*.csv)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);

            QVector<QVector<TileType>> tiles_type_map;

            while (!in.atEnd())
            {
                QString line = in.readLine();
                QStringList values = line.split(",", Qt::SkipEmptyParts);

                QVector<TileType> row;

                for (const QString &value : values)
                {
                    bool conversionOK;
                    uint8_t int_value = value.toInt(&conversionOK, 10);
                    if (conversionOK)
                    {
                        row.append(static_cast<TileType>(int_value));
                    }
                    else
                    {
                        qDebug() << "Error converting value to int:" << value;
                    }
                }

                tiles_type_map.append(row);
            }

            file.close();

            map->loadMap(tiles_type_map);

            qDebug() << "Map loaded from CSV file:" << fileName;
        }
        else
        {
            qDebug() << "Unable to open file:" << fileName;
        }
    }
}
