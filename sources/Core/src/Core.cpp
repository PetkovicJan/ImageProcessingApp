#include <Core/Core.hpp>

#include <vector>
#include <utility>

namespace detail
{
  bool is_in_range(ptrdiff_t i, ptrdiff_t lo, ptrdiff_t hi)
  {
    return i >= lo && i < hi;
  }

  ptrdiff_t clamp_index(ptrdiff_t index, ptrdiff_t size)
  {
    return index < 0 ? 0 : (index >= size ? size - 1 : index);
  }

  int8_t canny_get_angle_bin(double angle)
  {
    const auto bin_width = constants::quarter_pi;

    angle += constants::eighth_pi;
    if (angle < 0.) angle += constants::pi;
    else if (angle >= constants::pi) angle -= constants::pi;

    return static_cast<int8_t>(angle / bin_width);
  }

  void track_edge_hysteresis_impl(Image2d<unsigned char>& dst, Position start)
  {
    const auto w = dst.width();
    const auto h = dst.height();

    const std::vector<Position> moore_neighbors = {
      Position(-1, -1), Position(-1, 0), Position(-1, 1), Position(0, 1),
      Position(1, 1), Position(1, 0), Position(1, -1), Position(0, -1) };

    std::vector<Position> hysteresis = { start };
    while (!hysteresis.empty())
    {
      const auto curr = hysteresis.back();
      hysteresis.pop_back();

      for (const auto nbh : moore_neighbors)
      {
        const auto nbh_pos = curr + nbh;
        if (dst.isValid(nbh_pos) && dst(nbh_pos) == 1)
        {
          dst(nbh_pos) = 255;
          hysteresis.push_back(nbh_pos);
        }
      }
    }
  }

  void hysteresis_edge_tracking(Image2d<unsigned char>& dst)
  {
    foreach2d(dst, y, x)
    {
      const auto pos = Position(y, x);
      const auto val = dst(pos);
      if (val == 2)
      {
        dst(pos) = 255;
        track_edge_hysteresis_impl(dst, pos);
      }
    }
  }
}

FilterOp::FilterOp(ptrdiff_t kernel_radius_x, ptrdiff_t kernel_radius_y, float sigma_x, float sigma_y) :
  kernel_radius_x_(kernel_radius_x), kernel_radius_y_(kernel_radius_y), sigma_x_(sigma_x), sigma_y_(sigma_y)
{
}

void FilterOp::perform(Image2d<float> const& in, Image2d<float>& out) const
{
  gauss_filter(in, kernel_radius_y_, kernel_radius_x_, sigma_y_, sigma_x_, BorderCondition::BC_CLAMP, out);
}

void OperationChain::addOperation(std::unique_ptr<Operation> op)
{
  chain_.emplace_back(std::move(op));
}

void OperationChain::executeChain(Image2d<float> const& in, Image2d<float>& out) const
{
  if (chain_.size() == 0)
  {
    fill(out, in);
  }
  else if (chain_.size() == 1)
  {
    chain_.front()->perform(in, out);
  }
  else
  {
    Image2d<float> tmp1(in.size());
    fill(tmp1, in);

    Image2d<float> tmp2(in.size());

    for (auto& op : chain_)
    {
      op->perform(tmp1, tmp2);
      std::swap(tmp1, tmp2);
    }

    out = std::move(tmp1);
  }
}
