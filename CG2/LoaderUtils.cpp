#include "LoaderUtils.h"

auto OpenFile(std::string path) -> std::ifstream {
	return std::ifstream(std::move(path));
}

auto CreateLoader(std::string path) -> Loader {
	return Loader(std::move(path));
}

auto CreateLoaders(std::vector<std::string> paths) -> std::vector<Loader> {
	std::vector<Loader> loaders;
	loaders.reserve(paths.size());
	std::transform(paths.begin(), paths.end(), std::back_inserter(loaders), CreateLoader);
	auto removed = std::remove_if(loaders.begin(), loaders.end(), [](Loader& loader) { return !loader.isLoad(); });
	loaders.erase(removed, loaders.end());
	return loaders;
}

auto LoadPaths(std::istream& stream) -> std::vector<std::string> {
	std::vector<std::string> result;
	std::string temp;
	while (std::getline(stream, temp)) {
		result.push_back(temp);
	}
	return result;
}

auto CreateModel(Loader loader) -> Model2D
{
	return Model2D( loader.TakeVerticles(), loader.TakeIndices() );
}

auto CreateModels(std::vector<Loader> loaders) -> std::vector<Model2D> {
	std::vector<Model2D> result;
	result.reserve(loaders.size());
	std::transform(loaders.begin(), loaders.end(), std::back_inserter(result), CreateModel);
	return result;
}

auto CreateScene(std::vector<Model2D> models) -> Scene2D
{
	Scene2D scene;
	auto LoadModelToScene = [&scene](Model2D& model) {
		scene.addObject(std::move(model));
	};
	std::for_each(models.begin(), models.end(), LoadModelToScene);
	return scene;
}

auto LoadScene(std::string path) -> Scene2D
{
	auto file = OpenFile(std::move(path));
	auto paths = LoadPaths(file);
	auto loaders = CreateLoaders(std::move(paths));
	auto models = CreateModels(std::move(loaders));
	auto scene = CreateScene(std::move(models));
	return scene;
}
