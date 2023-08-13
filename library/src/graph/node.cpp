#include <gin/graph/node.hpp>
#include <gin/math/math.hpp>

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
