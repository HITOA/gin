#include <gin/graph/node.hpp>
#include <gin/math/math.hpp>
#include <gin/field/sampler.hpp>

void Gin::Graph::Node::Initialize(GraphContext ctx) {}

void Gin::Graph::Node::Initialize(GraphContext ctx, Thread::ThreadPool& pool)
{
    Initialize(ctx);
}

void Gin::Graph::Node::Clear() {
    /*for (auto& output : outputs) {
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
    }*/
}

void Gin::Graph::Node::Update() {}

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

        //Constant Field
        if (input->GetType() == GetPortTypeInfo<Field::Sampler<float>>()) {
            Field::Sampler<float>* f = (Field::Sampler<float>*)input->GetProperty();
            if (f->IsFieldOfType<Field::ConstantField<float>>())
                data[input->GetName()] = f->GetScalar(0, 0, 0);
        }

        //Dynamic Field
        if (input->GetType() == GetPortTypeInfo<Field::DynamicSampler>()) {
            Field::DynamicSampler* f = (Field::DynamicSampler*)input->GetProperty();
            if (f->IsFieldOfType<Field::ConstantField<float>>())
                data[input->GetName()] = f->GetScalar(0, 0, 0);
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

            //Constant Field
            if (input->GetType() == GetPortTypeInfo<Field::Sampler<float>>()) {
                Field::Sampler<float>* f = (Field::Sampler<float>*)input->GetProperty();
                if (f->IsFieldOfType<Field::ConstantField<float>>())
                    f->GetField<Field::ConstantField<float>>()->Get() = data[input->GetName()];
            }

            if (input->GetType() == GetPortTypeInfo<Field::DynamicSampler>()) {
                Field::DynamicSampler* f = (Field::DynamicSampler*)input->GetProperty();
                if (f->IsFieldOfType<Field::ConstantField<float>>())
                    f->GetField<Field::ConstantField<float>>()->Get() = data[input->GetName()];
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
