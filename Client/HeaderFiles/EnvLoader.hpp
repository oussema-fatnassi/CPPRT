#ifndef ENV_LOADER_H
#define ENV_LOADER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

bool loadEnvFile(const QString &filePath);

#endif // ENV_LOADER_H
