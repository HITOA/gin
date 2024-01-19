#include <gin/graph/node.hpp>
#include <gin/math/math.hpp>
#include <gin/spatial/spatial.hpp>

void Gin::Graph::Node::Initialize(GraphContext ctx)
{
    this->ctx = ctx;

    Eigen::Vector3<double> t{ ctx.bounds.extent * 2.0 / ctx.scale };
    Eigen::Vector3<int> size = Math::Ceil<double, int, 3>(t);

    for (auto& output : outputs) {
        if ((int)(output->GetType().type) & (int)PortType::Spatial) {
            Spatial::BaseSpatial* spatial = (Spatial::BaseSpatial*)output->GetProperty();
            spatial->Resize(size.x(), size.y(), size.z());
        }
    }

    for (auto& input : inputs) {
        if ((int)(input->GetType().type) & (int)PortType::Spatial) {
            Spatial::BaseSpatial* spatial = (Spatial::BaseSpatial*)input->GetProperty();
            spatial->Resize(size.x(), size.y(), size.z());
        }
    }
}

void Gin::Graph::Node::Initialize(GraphContext ctx, Thread::ThreadPool& pool)
{
    this->ctx = ctx;

    Eigen::Vector3<double> t{ ctx.bounds.extent * 2.0 / ctx.scale };
    Eigen::Vector3<int> size = Math::Ceil<double, int, 3>(t);

    for (auto& output : outputs) {
        if ((int)(output->GetType().type) & (int)PortType::Spatial) {
            Spatial::BaseSpatial* spatial = (Spatial::BaseSpatial*)output->GetProperty();
            spatial->Resize(size.x(), size.y(), size.z());
        }
    }

    /*for (auto& input : inputs) {
        if ((int)(input->GetType().type) & (int)PortType::Spatial) {
            Spatial::BaseSpatial* spatial = (Spatial::BaseSpatial*)input->GetProperty();
            spatial->Resize(size.x(), size.y(), size.z());
        }
    }*/
}

void Gin::Graph::Node::Clear() {
    for (auto& output : outputs) {
        if ((int)(output->GetType().type) & (int)PortType::Spatial) {
            Spatial::BaseSpatial* spatial = (Spatial::BaseSpatial*)output->GetProperty();
            spatial->Resize(1, 1, 1);
        }
    }

    for (auto& input : inputs) {
        if ((int)(input->GetType().type) & (int)PortType::Spatial) {
            Spatial::BaseSpatial* spatial = (Spatial::BaseSpatial*)input->GetProperty();
            spatial->Resize(1, 1, 1);
        }
    }
}

void Gin::Graph::Node::Execute(GraphContext ctx, Thread::ThreadPool& pool)
{
    Execute(ctx);
}

nlohmann::json Gin::Graph::Node::Serialize()
{
    nlohmann::json data = nlohmann::json::object();

    for (auto& input : inputs) {
        //Number
        if (input->GetType() == GetPortTypeInfo<int>()) {
            data[input->GetName()] = *(int*)input->GetProperty();
            continue;
        }
        if (input->GetType() == GetPortTypeInfo<float>()) {
            data[input->GetName()] = *(float*)input->GetProperty();
            continue;
        }
        if (input->GetType() == GetPortTypeInfo<double>()) {
            data[input->GetName()] = *(double*)input->GetProperty();
            continue;
        }

        //Spatial Number
        if (input->GetType() == GetPortTypeInfo<Spatial::Spatial<int>>()) {
            data[input->GetName()] = (*(Spatial::Spatial<int>*)input->GetProperty())[0];
            continue;
        }
        if (input->GetType() == GetPortTypeInfo<Spatial::Spatial<float>>()) {
            data[input->GetName()] = (*(Spatial::Spatial<float>*)input->GetProperty())[0];
            continue;
        }
        if (input->GetType() == GetPortTypeInfo<Spatial::Spatial<double>>()) {
            data[input->GetName()] = (*(Spatial::Spatial<double>*)input->GetProperty())[0];
            continue;
        }
    }

    return data;
}

void Gin::Graph::Node::Deserialize(nlohmann::json data)
{
    for (auto& input : inputs) {
        if (data.count(input->GetName())) {
            //Number
            if (input->GetType() == GetPortTypeInfo<int>()) {
                *(int*)input->GetProperty() = data[input->GetName()];
                continue;
            }
            if (input->GetType() == GetPortTypeInfo<float>()) {
                *(float*)input->GetProperty() = data[input->GetName()];
                continue;
            }
            if (input->GetType() == GetPortTypeInfo<double>()) {
                *(double*)input->GetProperty() = data[input->GetName()];
                continue;
            }

            //Spatial Number
            if (input->GetType() == GetPortTypeInfo<Spatial::Spatial<int>>()) {
                (*(Spatial::Spatial<int>*)input->GetProperty())[0] = data[input->GetName()];
                continue;
            }
            if (input->GetType() == GetPortTypeInfo<Spatial::Spatial<float>>()) {
                (*(Spatial::Spatial<float>*)input->GetProperty())[0] = data[input->GetName()];
                continue;
            }
            if (input->GetType() == GetPortTypeInfo<Spatial::Spatial<double>>()) {
                (*(Spatial::Spatial<double>*)input->GetProperty())[0] = data[input->GetName()];
                continue;
            }
        }
    }
}

size_t Gin::Graph::Node::AddInputPort(std::shared_ptr<Port> port)
{
    if (inputs.size() >= MAX_INPUT_PORT)
        throw std::runtime_error{ "Input limit reached." };

    size_t c = inputs.size();
    inputs.push_back(port);
    return c;
}

Gin::Graph::Port& Gin::Graph::Node::GetInputPort(size_t idx)
{
    if (idx >= inputs.size())
        throw std::invalid_argument{ "Invalid input index." };

    return *inputs[idx];
}

size_t Gin::Graph::Node::GetInputPortCount()
{
    return inputs.size();
}

size_t Gin::Graph::Node::AddOutputPort(std::shared_ptr<Port> port)
{
    if (outputs.size() >= MAX_OUTPUT_PORT)
        throw std::runtime_error{ "Output limit reached." };

    size_t c = outputs.size();
    outputs.push_back(port);
    return c;
}

Gin::Graph::Port& Gin::Graph::Node::GetOutputPort(size_t idx)
{
    if (idx >= outputs.size())
        throw std::invalid_argument{ "Invalid output index." };

    return *outputs[idx];
}

size_t Gin::Graph::Node::GetOutputPortCount()
{
    return outputs.size();
}

void Gin::Graph::Node::SetPath(const std::string& path)
{
    this->path = path;
}

std::string Gin::Graph::Node::GetPath()
{
    return path;
}
