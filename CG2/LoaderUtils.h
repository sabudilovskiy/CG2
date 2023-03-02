#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "Loader.h"
#include "Model2D.h"
#include "Scene2D.h"

auto OpenFile(std::string path) -> std::ifstream;

auto CreateLoader(std::string path) -> Loader;

auto LoadPaths(std::istream& stream) -> std::vector<std::string>;

auto CreateLoaders(std::vector<std::string> paths) -> std::vector<Loader>;

auto CreateModel(Loader loader) -> Model2D;

auto CreateModels(std::vector<Loader> loaders) -> std::vector<Model2D>;

auto CreateScene(std::vector<Model2D> models) -> Scene2D;

auto LoadScene(std::string path) -> Scene2D;