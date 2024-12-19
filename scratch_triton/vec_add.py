import torch

import triton
import triton.language as tl

DEVICE = torch.device('cuda:0')

@triton.jit
def add_kernel(x, y, z, n_elements, BLOCK_SIZE: tl.constexpr) :
    pid = tl.program_id(axis=0)
    block_start = pid * BLOCK_SIZE
    offsets = block_start + tl.arange(0, BLOCK_SIZE)
    mask = offsets < n_elements
    x = tl.load(x + offsets, mask = mask)
    y = tl.load(y + offsets, mask = mask)
    tl.store(z + offsets, x + y, mask=mask)

def add(x: torch.Tensor, y: torch.Tensor):
    z = torch.empty_like(x)
    n_elements = z.numel()
    grid = lambda meta: (triton.cdiv(n_elements, meta['BLOCK_SIZE']), )
    add_kernel[grid](x, y, z, n_elements, BLOCK_SIZE=1024)
    return z

def main() :
    size = 327680
    torch.manual_seed(0)
    x = torch.rand(size, device = DEVICE)
    y = torch.rand(size, device = DEVICE)
    z_torch = x + y
    z_tr = add(x, y)

    print(f'Max diff of add result from torch vs triton are:')
    print(f"{torch.max(torch.abs(z_torch - z_tr))}")    

main()
