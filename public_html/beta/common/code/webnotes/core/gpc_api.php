<?php
        # phpWebNotes - a php based note addition system
        # Copyright (C) 2000-2002 Webnotes Team - webnotes-devel@sourceforge.net
        # This program is distributed under the terms and conditions of the GPL
        # See the files README and LICENSE for details

	# --------------------------------------------------------
	# $Id$
	# --------------------------------------------------------

	###########################################################################
	# GET, POST, and Cookie API
	###########################################################################

	# ---------------
	# Retrieve a GPC variable.
	# If the variable is not set, the default is returned. 
	# If magic_quotes_gpc is on, slashes will be stripped from the value before being returned.
	#
	#  You may pass in any variable as a default (including null) but if
	#  you pass in *no* default then an error will be triggered if the field
	#  cannot be found
	function gpc_get( $p_var_name, $p_default = null ) {
		# simulate auto-globals from PHP v4.1.0 (see also code in php_api.php)
		if ( ! php_version_at_least( '4.1.0' ) ) {
			global $_POST, $_GET;
		}

		if ( isset( $_POST[$p_var_name] ) ) {
			$t_result = gpc_strip_slashes( $_POST[$p_var_name] );
		} else if ( isset( $_GET[$p_var_name] ) ) {
			$t_result = gpc_strip_slashes( $_GET[$p_var_name] );
		} else if ( func_num_args() > 1 ) { #check for a default passed in (allowing null)
			$t_result = $p_default;
		} else {
			#trigger_error(ERROR_GPC_VAR_NOT_FOUND, ERROR);
			echo "Variable '$p_var_name' not found";
			$t_result = null;
		}
		
		return $t_result;
	}
	# -----------------
	# Retrieve a string GPC variable. Uses gpc_get().
	#  If you pass in *no* default, an error will be triggered if
	#  the variable does not exist
	function gpc_get_string( $p_var_name, $p_default = null ) {
		# Don't pass along a default unless one was given to us
		#  otherwise we prevent an error being triggered
		if ( func_num_args() > 1 ) {
			$t_result = gpc_get( $p_var_name, $p_default );
		} else {
			$t_result = gpc_get( $p_var_name );
		}

		if ( is_array( $t_result ) ) {
			#trigger_error( ERROR_GPC_ARRAY_UNEXPECTED, ERROR );e
			echo "Unexpected array '$p_var_name'.";
		}

		return $t_result;
	}
	# ------------------
	# Retrieve an integer GPC variable. Uses gpc_get().
	#  If you pass in *no* default, an error will be triggered if
	#  the variable does not exist
	function gpc_get_int( $p_var_name, $p_default = null ) {
		# Don't pass along a default unless one was given to us
		#  otherwise we prevent an error being triggered
		if ( func_num_args() > 1 ) {
			$t_result = gpc_get( $p_var_name, $p_default );
		} else {
			$t_result = gpc_get( $p_var_name );
		}

		if ( is_array( $t_result ) ) {
			#trigger_error( ERROR_GPC_ARRAY_UNEXPECTED, ERROR );
			echo "Unexpected array '$p_var_name'.";
		}

		return (integer)$t_result;
	}
	# ------------------
	# Retrieve a boolean GPC variable. Uses gpc_get().
	#  If you pass in *no* default, false will be used
	function gpc_get_bool( $p_var_name, $p_default = false ) {
		$t_result = gpc_get( $p_var_name, $p_default );

		if ( $t_result === $p_default ) {
			return $p_default;
		} else {
			if ( is_array( $t_result ) ) {
				#trigger_error( ERROR_GPC_ARRAY_UNEXPECTED, ERROR );
				echo "Unexpected array '$p_var_name'.";
			}

			return gpc_string_to_bool( $t_result );
		}
	}

	#===================================
	# Array Functions
	#===================================

	# ------------------
	# Retrieve a atring array GPC variable.  Uses gpc_get().
	#  If you pass in *no* default, an error will be triggered if
	#  the variable does not exist
	function gpc_get_string_array( $p_var_name, $p_default = null ) {
		# Don't pass along a default unless one was given to us
		#  otherwise we prevent an error being triggered
		if ( func_num_args() > 1 ) {
			$t_result = gpc_get( $p_var_name, $p_default );
		} else {
			$t_result = gpc_get( $p_var_name );
		}
		
		if ( ! is_array( $t_result ) ) {
			#trigger_error( ERROR_GPC_ARRAY_EXPECTED, ERROR);
			echo "Unexpected array '$p_var_name'.";
		}
		
		return $t_result;
	}
	# ------------------
	# Retrieve an integer array GPC variable.  Uses gpc_get().
	#  If you pass in *no* default, an error will be triggered if
	#  the variable does not exist
	function gpc_get_int_array( $p_var_name, $p_default = null ) {
		# Don't pass along a default unless one was given to us
		#  otherwise we prevent an error being triggered
		if ( func_num_args() > 1 ) {
			$t_result = gpc_get( $p_var_name, $p_default );
		} else {
			$t_result = gpc_get( $p_var_name );
		}
		
		if ( ! is_array( $t_result ) ) {
			#trigger_error( ERROR_GPC_ARRAY_EXPECTED, ERROR);
			echo "Unexpected array '$p_var_name'.";
		} 

		for ( $i=0 ; $i < sizeof( $t_result ) ; $i++ ) {
			$t_result[$i] = (integer)$t_result[$i];
		}

		return $t_result;
	}
	# ------------------
	# Retrieve a boolean array GPC variable.  Uses gpc_get().
	#  If you pass in *no* default, an error will be triggered if
	#  the variable does not exist
	function gpc_get_bool_array( $p_var_name, $p_default = null ) {
		# Don't pass along a default unless one was given to us
		#  otherwise we prevent an error being triggered
		if ( func_num_args() > 1 ) {
			$t_result = gpc_get( $p_var_name, $p_default );
		} else {
			$t_result = gpc_get( $p_var_name );
		}
		
		if ( ! is_array( $t_result ) ) {
			#trigger_error( ERROR_GPC_ARRAY_EXPECTED, ERROR);
			echo "Unexpected array '$p_var_name'.";
		}

		for ( $i=0 ; $i < sizeof( $t_result ) ; $i++ ) {
			$t_result[$i] = gpc_string_to_bool( $t_result[$i] );
		}

		return $t_result;
	}

	#===================================
	# Cookie Functions
	#===================================

	# ------------------
	# Retrieve a cookie variable
	#  You may pass in any variable as a default (including null) but if
	#  you pass in *no* default then an error will be triggered if the cookie
	#  cannot be found
	function gpc_get_cookie( $p_var_name, $p_default = null ) {
		# simulate auto-globals from PHP v4.1.0 (see also code in php_api.php)
		if ( ! php_version_at_least( '4.1.0' ) ) {
			global $_COOKIE;
		}

		if ( isset( $_COOKIE[$p_var_name] ) ) {
			$t_result = gpc_strip_slashes( $_COOKIE[$p_var_name] );
		} else if ( func_num_args() > 1 ) { #check for a default passed in (allowing null)
			$t_result = $p_default;
		} else {
			#trigger_error(ERROR_GPC_VAR_NOT_FOUND, ERROR);
			echo "Variable '$p_var_name' not found";
			$t_result = null;
		}
		
		return $t_result;
	}

	#===================================
	# Helper Functions
	#===================================

	# ------------------
	# Convert a string to a bool
	function gpc_string_to_bool( $p_string ) {
		if ( 0 == strcasecmp( 'off', $p_string ) ||
			 0 == strcasecmp( 'no', $p_string ) ||
			 0 == strcasecmp( 'false', $p_string ) ||
			 0 == strcasecmp( '0', $p_string ) ) {
			return false;
		} else {
			return true;
		}
	}

	# ------------------
	# Strip slashes if necessary (supports arrays)
	function gpc_strip_slashes( $p_var ) {
		if (get_magic_quotes_gpc() == 0) {
			return $p_var;
		} else if ( ! is_array( $p_var ) ){
			return stripslashes( $p_var );
		} else {
			for ( $i=0 ; $i < sizeof( $p_var ) ; $i++ ) {
				$p_var[$i] = stripslashes( $p_var[$i] );

				return $p_var;
			}
		}
	}
?>
