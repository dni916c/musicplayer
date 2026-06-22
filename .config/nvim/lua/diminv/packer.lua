-- This file can be loaded by calling `lua require('plugins')` from your init.vim

-- Only required if you have packer configured as `opt`
vim.cmd [[packadd packer.nvim]]

return require('packer').startup(function(use)
  -- Packer can manage itself
  use 'wbthomason/packer.nvim'

  use {
	  'nvim-telescope/telescope.nvim', tag = '0.1.3',
	  requires = {{'nvim-lua/plenary.nvim'}}
  }

  use 'folke/tokyonight.nvim'

  use {
    'nvim-treesitter/nvim-treesitter',
    branch = 'master',
    run = ':TSUpdate'
  }
  use('neovim/nvim-lspconfig')

  use({
    'saghen/blink.cmp',
    branch = 'v1',                          -- pin v1; the default branch
    requires = { 'rafamadriz/friendly-snippets' },
   })

end)
