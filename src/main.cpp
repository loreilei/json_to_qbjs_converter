#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

#include <iostream>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("json_to_qbjs_converter");
  QCoreApplication::setApplicationVersion("0.1.0");

  QCommandLineParser parser;
  parser.setApplicationDescription(
      "Read JSON files and saves them as Qt's internal binary JSON format "
      "(that got deprecated from Qt6\n");
  parser.addHelpOption();
  parser.addVersionOption();

  constexpr auto jsonFilesFolderOption = "json_files_folder";
  constexpr auto jsonFilesFolderValue = "json_files_folder_value";
  constexpr auto qbjsFilesFolderOption = "qbjs_files_folder";
  constexpr auto qbjsFilesFolderValue = "qbjs_files_folder_value";

  parser.addOptions({{jsonFilesFolderOption,
                      "Full path to folder holding the JSON files to convert",
                      jsonFilesFolderValue},
                     {qbjsFilesFolderOption,
                      "Full path to folder where converted files should be put",
                      qbjsFilesFolderValue}});

  parser.process(app);

  if (!parser.isSet(jsonFilesFolderOption) ||
      !parser.isSet(qbjsFilesFolderOption)) {
    std::cerr << "Missing option(s):";
    auto printMissingOption = [&parser](const auto option) {
      if (!parser.isSet(option)) {
        std::cerr << option << " ";
      }
    };
    printMissingOption(jsonFilesFolderOption);
    printMissingOption(qbjsFilesFolderOption);
    std::cerr << std::endl;
    std::cerr << "Run with --help to see more information" << std::endl;

    return -1;
  }

  const QString json_files_folder = parser.value(jsonFilesFolderOption);
  const QString qbjs_files_folder = parser.value(qbjsFilesFolderOption);

  if (!QFile::exists(qbjs_files_folder)) {
    std::cout << "Creating qbjs output folder" << std::endl;
    QDir().mkpath(qbjs_files_folder);
  }

  auto convert = [&json_files_folder,
                  &qbjs_files_folder](const auto &json_filename) {
    const auto base_filename = QFileInfo(json_filename).baseName();
    const auto full_json_filename =
        json_files_folder + QDir::separator() + json_filename;
    QFile json_file(full_json_filename);

    if (!json_file.exists()) {
      std::cerr << "Input JSON file " << full_json_filename.toStdString()
                << " not found" << std::endl;
      return;
    }

    QByteArray json_data;
    if (json_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      json_data = json_file.readAll();
    }

    auto doc = QJsonDocument::fromJson(json_data);

    const auto qbjs_filename = base_filename + QString::fromStdString(".qbjs");
    const auto full_qbjs_filename =
        qbjs_files_folder + QDir::separator() + qbjs_filename;

    if (QFileInfo::exists(full_qbjs_filename)) {
      QFile::remove(full_qbjs_filename);
    }

    QFile out_file(full_qbjs_filename);
    if (out_file.open(QIODevice::ReadWrite)) {
      out_file.write(doc.toBinaryData());
      std::cout << "Converted " << json_filename.toStdString() << " -> "
                << qbjs_filename.toStdString() << std::endl;
    } else {
      std::cerr << "Couldn't convert " << json_filename.toStdString() << " -> "
                << qbjs_filename.toStdString() << std::endl;
    }
  };

  const auto files = QDir(json_files_folder).entryList({"*.json"}, QDir::Files);

  if (files.empty()) {
    std::cerr << "No JSON files to convert in "
              << json_files_folder.toStdString() << std::endl;
    return -1;
  }

  for (const auto &file : files) {
    convert(file);
  }

  return 0;
}
