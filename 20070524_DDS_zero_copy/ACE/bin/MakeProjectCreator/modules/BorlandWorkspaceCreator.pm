package BorlandWorkspaceCreator;

# ************************************************************
# Description   : A Borland Workspace (Makefile.bor) creator
# Author        : Chad Elliott
# Create Date   : 7/02/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;
use File::Basename;

use BorlandProjectCreator;
use WorkspaceCreator;

use vars qw(@ISA);
@ISA = qw(WorkspaceCreator);

# ************************************************************
# Data Section
# ************************************************************

my($max_line_length) = 32767; ## Borland Make's maximum line length
my(@targets) = ('clean', 'realclean', 'install');

# ************************************************************
# Subroutine Section
# ************************************************************

sub workspace_file_name {
  my($self) = shift;
  return $self->get_modified_workspace_name('Makefile', '.bor');
}


sub workspace_per_project {
  #my($self) = shift;
  return 1;
}


sub pre_workspace {
  my($self) = shift;
  my($fh)   = shift;
  my($crlf) = $self->crlf();

  print $fh '#----------------------------------------------------------------------------', $crlf,
            '#       Borland Workspace', $crlf,
            '#', $crlf,
            '# $Id$', $crlf,
            '#', $crlf,
            '# This file was generated by MPC.  Any changes made directly to', $crlf,
            '# this file will be lost the next time it is generated.', $crlf,
            '#', $crlf,
            '# MPC Command:', $crlf,
            "# $0 @ARGV", $crlf,
            '#----------------------------------------------------------------------------', $crlf,
            $crlf;
}


sub write_project_targets {
  my($self)   = shift;
  my($fh)     = shift;
  my($target) = shift;
  my($list)   = shift;
  my($crlf)   = $self->crlf();

  foreach my $project (@$list) {
    my($dir)    = $self->mpc_dirname($project);
    my($chdir)  = 0;
    my($back)   = '';
    my($cwd)    = $self->getcwd();

    ## If the directory isn't '.' then we need
    ## to figure out how to get back to our starting point
    if ($dir ne '.') {
      $chdir = 1;
      my($count) = ($dir =~ tr/\///) + 1;
      if ($dir =~ /^\.\.\//) {
        ## Find out how many directories we went down
        my($rel) = $dir;
        while($rel =~ s/^\.\.\///) {
        }
        my($down) = ($rel =~ tr/\///) + 1;

        ## Get $count - $down parts of the base of the current directory
        $rel = $cwd;
        my($index) = length($rel);
        for(my $i = $down; $i < $count; $i++) {
          $index = rindex($rel, '/', $index - 1);
        }
        if ($index > -1) {
          $rel = substr($rel, $index + 1);
        }
        $back = ('../' x $down) . $rel;
      }
      else {
        $back = ('../' x $count);
      }
    }

    print $fh ($chdir ? "\t\@cd $dir$crlf" : '') .
              "\t\$(MAKE) -\$(MAKEFLAGS) \$(MAKE_FLAGS) -f " . basename($project) . " $target$crlf" .
              ($chdir ? "\t\@cd $back$crlf" : '');
  }
}


sub write_comps {
  my($self)     = shift;
  my($fh)       = shift;
  my($projects) = $self->get_projects();
  my($pjs)      = $self->get_project_info();
  my(%targnum)  = ();
  my(@list)     = $self->number_target_deps($projects, $pjs, \%targnum, 0);
  my($crlf)     = $self->crlf();
  my(@ltargets) = @targets;

  print $fh "!include <\$(ACE_ROOT)\\include\\makeinclude\\make_flags.bor>$crlf";

  ## Construct the "all" target
  my($all) = $crlf . 'all:';
  foreach my $project (@list) {
    $all .= " $$pjs{$project}->[0]";
  }
  if (length($all) < $max_line_length) {
    print $fh $all, $crlf;
  }
  else {
    unshift(@ltargets, 'all');
  }

  ## Print out all other targets here
  foreach my $target (@ltargets) {
    print $fh $crlf .
              "$target\:$crlf";
    $self->write_project_targets($fh, $target, \@list);
  }

  ## Print out each target separately
  foreach my $project (@list) {
    print $fh $crlf . $$pjs{$project}->[0] . ':';
    if (defined $targnum{$project}) {
      foreach my $number (@{$targnum{$project}}) {
        print $fh " $$pjs{$list[$number]}->[0]";
      }
    }

    print $fh $crlf;
    $self->write_project_targets($fh, 'all', [ $project ]);
  }

  ## Print out the project_name_list target
  print $fh $crlf . "project_name_list:$crlf";
  foreach my $project (sort @list) {
    print $fh "\t\@echo $$pjs{$project}->[0]$crlf";
  }
}



1;
