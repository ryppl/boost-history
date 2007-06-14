#!/usr/bin/perl -w

=head1 NAME

 doxydoc.pl - Process comments in a source file, from our own C++ format,
              changing the syntax to make it acceptable to doxygen

=head1 SYNOPSIS

 cpp2doxy.pl file.hpp > file..hpp

=head1 DESCRIPTION
=over 4

This will reformat comments from a source file. Comments follow the 
succinct and readable notation and are converted into the clumsy and
less readable doxygen notation.  Since we don't like @e, @code
@endcode, @a, etc., we use the following guidelines:

=item @DESCRIPTION starts the file-level comment.

=item section headers are started with three slashes followed by an
  underline:
      ///Main section header
      ///-------------------
=item paragraphs are implicitly started by an empty line (with only two slashes //)

=item typewriter font triggered by 'variable'

=item emphasis is *strong*,

=item underlined is _underlined_ and

=item code is delimited by lines consisting only with '//..', which will
  create a box of fixed-width

=item all C++ comments starting with // are automatically converted to
  Doxygen comments after the fact (i.e. //!< in doygen notation).  

This makes the source code much more readable.

=back
=head1 Filter
=over 4


=back
=head1 AUTHOR
=over 4

Herve Bronnimann, Huseyin Akcan.

=cut

sub process_font {
    my $content = shift;

    my $output = "";
    while ($content =~ m/([^']*)\'([^']*)\'(.*)/) {
        local $textpart = $1;
        local $codepart = $2;
        local $remainder = $3;

        # Process references
        $textpart =~ s/\[([^\]]*)\]/\\ref $1/g;

        # Guard against template and comparisons
        $textpart =~ s/</&lt;/g;
        $textpart =~ s/>/&gt;/g;

        # Process emphasis in italics
        $textpart =~ s/\*([^*]*)\*/<i>$1<\/i>/g;

        # Process bold
        $textpart =~ s/\!([^!]*)\!/<b>$1<\/b>/g;

        # Process underline
        $textpart =~ s/_([^_]*)_/<u>$1<\/u>/g;

        # Process typewriter font
        $codepart = "<code>".$codepart."<\/code>";

        $output  = $output.$textpart.$codepart;
        $content = $remainder;
    }

    # Process references
    $content =~ s/\[([^\]]*)\]/\\ref $1/g;

    # Guard against template and comparisons
    $content =~ s/</&lt;/g;
    $content =~ s/>/&gt;/g;

    # Process emphasis in italics
    $content =~ s/\*([^*]*)\*/<i>$1<\/i>/g;

    # Process bold
    $content =~ s/\!([^!]*)\!/<b>$1<\/b>/g;

    # Process underline
    $content =~ s/_([^_]*)_/<u>$1<\/u>/g;

    return $output.$content;
}

my $finishedTopLevel = 0; # to avoid parsing later comments as if preamble
my $conceptMode  = 0;     # serves to remember if a @CONCEPT command was parsed
my $conceptLabel = "";    # and if so, the concept name
my $conceptRef   = 0;     # numbers the section references
my $classOpen    = 0;     # serves to know if we're within a class body
my $className    = "";    # and if so, the class name
my $groupOpen    = 0;     # serves to close a named group before opening another

sub process_toplevel_comment_line {
    my $content = shift;

    if ($content =~ m/\@CONCEPT: \[(.*)\] (.*)/) {
        print " * \\page ".$1." ".$2."\n";
        $conceptMode = 1;
        $conceptLabel = process_font($1);
        return;
    }

    $content = process_font($content);
 
    if ($content eq "") {
        # Empty line, convert to @par, unless next line is a section header
        print " *\n";
        my $newContent = <>;
        if ($newContent =~ m/^\/\/\/(.*)$/) {
            if ($conceptMode == 1) {
                print " * \\section ref".$conceptLabel.++$conceptRef." ".$1."\n";
            } else {
                print " * \@par $1\n";
            }
            my $underLine = <>;
            if ($underLine !~ m/\/\/\/-+/) {
                die ("0x1: syntax error, ///----... expected. at line ".$.."\n");
            }
            return;
        } elsif ($newContent =~ m/^\/\/(.*)$/) {
            process_toplevel_comment_line($1);
        } else {
            print $newContent;
        }
    } elsif ($content eq "..") {
        # Code toggle, parse code until second //..
        print " * \\code\n";
        while(my $lineCode = <>) {
            if ($lineCode =~ m/\/\/\.\./) {
                print " * \\endcode\n";
                return;
            }
            $lineCode =~ s/^\/\// * /;
            print $lineCode;
        }
    } elsif ($content =~ m/\@(.*):(.*)/) {
        if ($1 eq "DESCRIPTION") {
            print " * \\details ".$2."\n";
        } elsif ($1 eq "PURPOSE") {
            print " * \\brief ".$2."\n";
        } else {
            die("0x2: Unknown section comment".$1." at line ".$.."\n");
        }
    } elsif ($content =~ m/^\/(.*)/) {
        # Section header, print as @par and ignore next line (ref is line no)
        print " * \\section ref".$conceptLabel.++$conceptRef." ".$1."\n";
        my $underLine = <>;
        if ($underLine !~ m/\/\/\/-+/) {
           die ("0x3: syntax error, ///----... expected at line ".$.."\n");
        }
    }
    else {
        print " * ".$content."\n";
    }
}

sub process_class_comment_line {
    my $prefix = shift;
    my $content = shift;

    $content = process_font($content);

    if ($content eq "") {
        # Empty line, convert to @par
        print $prefix." *< \@par\n";
    }
    elsif ($content =~ m/(TYPE|DATA|CREATOR|MANIP|ACCESS)/) {
        if ($groupOpen) {
            print $prefix."//\@\}\n";
        }
        $groupOpen = 1;
        if ($content =~ m/TYPE/) {
            print $prefix."//\@\{\n".
                  $prefix.'/** @name Types'." */\n";
        } elsif ($content =~ m/DATA/) {
            print $prefix."//\@\{\n".
                  $prefix.'/** @name Data members'." */\n";
        } elsif ($content =~ m/CREATOR/) {
            print $prefix."//\@\{\n".
                  $prefix.'/** @name Constructors, destructor'." */\n";
        } elsif ($content =~ m/MANIP/) {
            print $prefix."//\@\{\n".
                  $prefix.'/** @name Manipulators'." */\n";
        } elsif ($content =~ m/ACCESS/) {
            print $prefix."//\@\{\n".
                  $prefix.'/** @name Accessors'." */\n";
        } else {
            die("0x2: Unknown group comment".$1." at line ".$.."\n");
        }
    }
    elsif ($content eq "..") {
        # Code toggle, parse code until second //..
        print $prefix." * \\code\n";
        while(my $lineCode = <>) {
            if ($lineCode =~ m/\/\/\.\./) {
                print $prefix." * \\endcode\n";
                return;
            }
            s/^\/\//^\/\/\//;
            print $prefix." * ".$lineCode;
        }
    }
    else {
        print $prefix." * ".$content."\n";
    }
}

sub process_class_line {
    my $classLine = shift;

    if ($classLine =~ m/^( *)\/\/(.*)$/) {
        # C++-style comment within class definition scope, process accordingly.
        # Special group comments first.
        my $prefix = $1;
        my $content = $2;
        if ($content =~ m/^ *(public|protected|private)/) {
            if ($groupOpen) {
                print $prefix."//\@\}\n";
                $groupOpen = 0;
            }
            print $content;
        } elsif ($content =~ m/(NOT IMPL|TYPE|DATA|CREATOR|MANIP|ACCESS)/) {
            if ($groupOpen) {
                print $prefix."//\@\}\n";
            }
            $groupOpen = 1;
            if ($content =~ m/TYPE/) {
                print $prefix."//\@\{\n";
                print $prefix.'/** @name Member types'." */\n";
            } elsif ($content =~ m/DATA/) {
                print $prefix."//\@\{\n";
                print $prefix.'/** @name Data members'." */\n";
            } elsif ($content =~ m/CREATOR/) {
                print $prefix."//\@\{\n";
                print $prefix.'/** @name Constructors, destructor'." */\n";
            } elsif ($content =~ m/MANIP/) {
                print $prefix."//\@\{\n";
                print $prefix.'/** @name Manipulators'." */\n";
            } elsif ($content =~ m/ACCESS/) {
                print $prefix."//\@\{\n";
                print $prefix.'/** @name Accessors'." */\n";
            } elsif ($content =~ m/NOT IMPL/) {
                print $prefix."/* Not implemented */\n";
                $groupOpen = 0;
            } else {
                die("0x2: Unknown group comment".$1." at line ".$.."\n");
            }
        }
        else {
            # Normal comment, applies to either class, or entity above
            if ($className eq "") {
                print $1."/**< ".process_font($2)."\n";
            } else {
                print $1."/** \\class ".$className."\n".
                      $1."  *  ".process_font($2)."\n";
                $className = "";
            }
            my $comment = <>;
            while ($comment =~ m/^( *)\/\/(.*)$/) {
                process_class_comment_line($1, $2);
                $comment = <>;
            }
            print $1." **/\n";
            if ($comment !~ m/^( *)\};/) {
                process_class_line($comment);
            } else {
                if ($groupOpen == 1) {
                    print "    //\@\}\n";
                    $groupOpen = 0;
                }
                $classOpen = 0;
            }
        }
    } else {
        # C++ code or C-style comments within class definition scope.
        print $classLine;
    }
}

# MAIN LOOP

while(<>) {
    if ($_ =~ m/\/\/(.*)-\*- C\+\+ -\*-/) {
        # First line of file, generates 
        print "/** \\file ".$1."\n *\n";
        if (<> !~ m/^\/\/$/) {
            die("syntax error, empty comment line expected\n");
        }
    }
    elsif ($_ =~ m/^\/\/(.*)$/) {
        if ($finishedTopLevel == 0) {
            # C++-style top-level comment, process accordingly.
            my $comment = $_;
            while ($comment =~ m/^\/\/(.*)$/) {
                process_toplevel_comment_line($1);
                $comment = <>;
            }
            print " **/\n".$comment;
            $finishedTopLevel = 1;
        } else {
            print $_;
        }
    }
    elsif ($_ =~ m/^( *)(class|struct|union) +([a-zA-Z][a-zA-Z_]*)[^{]*\{/) {
        $classOpen = 1;
        $groupOpen = 0;
        $className = $3;
        print $_;

        my $classLine = <>;
        while ($classLine !~ m/^( *)\};/) {
            process_class_line($classLine);
            # The following is a dirty hack - we read the end of line already
            # and now we still must exit the loop...
            if (0 == $classOpen) {
                $classLine = "};\n";
            } else {
                $classLine = <>;
            }
        }
        if ($groupOpen == 1) {
            print "    //\@\}\n";
            $groupOpen = 0;
        }
        print $classLine;
        $classOpen = 0;
        $className = "";
    }
    else {
        # C++ code or C-style comments outside class definition scope.
        print $_;
    }
}
